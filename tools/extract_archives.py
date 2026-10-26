"""Extract AC3 numeric resources and original, single-channel XA sectors"""
import argparse
import hashlib
import json
import mmap
from pathlib import Path
import struct
from asset_roles import music_ids


SYNC = b'\x00' + b'\xff' * 10 + b'\x00'


def publish(path, data, verify, previous_hash=None):
    digest = hashlib.sha256(data).hexdigest()
    replace = False
    if path.exists():
        current_hash = hashlib.sha256(path.read_bytes()).hexdigest()
        if path.stat().st_size != len(data) or current_hash != digest:
            if verify or current_hash != previous_hash:
                raise ValueError(f'Modified output: {path}')
            replace = True
    elif verify:
        raise ValueError(f'Missing output: {path}')
    if not path.exists() or replace:
        path.parent.mkdir(parents=True, exist_ok=True)
        temporary = path.with_name(path.name + '.tmp')
        temporary.write_bytes(data)
        temporary.replace(path)
    return dict(bytes=len(data), sha256=digest)


def extract(root, discs, verify=False, refresh=False):
    previous = {}
    previous_audio = {}
    receipt = root / 'status/extracted-assets/extraction.json'
    if refresh and receipt.exists():
        old = json.loads(receipt.read_text())
        for disc in old['discs']:
            for archive in disc['archives']:
                for entry in archive['entries']:
                    if entry.get('path'):
                        previous[entry['path']] = entry['sha256']
                    if entry.get('xa'):
                        previous_audio[(disc['disc'], entry['index'])] = entry['xa']
    report = dict(schema=1, naming='Decimal local index; bit 0x800 selects ACE_SPB',
                  sp_layout='Unchanged raw 2352 sectors, selected channel plus channel-zero data; non-audio ACE_STREAM derivatives; BGM XA in MUSIC and other XA in DATA/ACE_XA',
                  xa_layout='Unchanged audio sectors, one file/channel/coding tuple, no transcoding', discs=[])
    for disc in discs:
        folder = root / 'bin/DATA' / str(disc)
        music = music_ids(folder / 'ACE3.BIN')
        result = dict(disc=disc, archives=[])
        for kind in ('B', 'S'):
            header = (folder / f'ACE.{kind}PH').read_bytes()
            if len(header) < 20 or struct.unpack_from('<I', header)[0] != 0x45334341:
                raise ValueError('Invalid archive header')
            count = struct.unpack_from('<I', header, 16)[0]
            if len(header) != 20 + count * 8:
                raise ValueError('Invalid archive index length')
            archive = folder / f'ACE.{kind}PB'
            entries = []
            with archive.open('rb') as source, mmap.mmap(source.fileno(), 0, access=mmap.ACCESS_READ) as data:
                identity = hashlib.sha256(data).hexdigest()
                for index, (word, start) in enumerate(struct.iter_unpack('<II', header[20:])):
                    entry = dict(index=index, id=index | (0x800 if kind == 'S' else 0),
                                 word=word, start_sector=start, present=bool(word & 0x80000000))
                    entries.append(entry)
                    if not entry['present']:
                        continue
                    target = folder / f'ACE_{kind}PB' / str(index)
                    if kind == 'B':
                        size = (word & 0x7FFFF) * 4
                        offset = start * 2048
                        if offset + size > len(data):
                            raise ValueError(f'Out-of-bounds BP resource {disc}:{index}')
                        entry.update(publish(target, data[offset:offset + size], verify))
                    else:
                        sectors = (word & 0x7FFFF) << ((word >> 24) & 7)
                        end = start + sectors
                        if end * 2352 > len(data):
                            raise ValueError(f'Out-of-bounds SP resource {disc}:{index}')
                        channel = (word >> 19) & 31
                        payload, stream_data, audio, metadata = bytearray(), bytearray(), bytearray(), bytearray()
                        tuple_ = None
                        first_audio = None
                        for sector in range(start, end):
                            offset = sector * 2352
                            block = data[offset:offset + 2352]
                            if block[:12] != SYNC or block[15] != 2 or block[16:20] != block[20:24]:
                                raise ValueError(f'Invalid raw sector {disc}:{sector}')
                            selected = block[17] == channel
                            if selected or (block[17] == 0 and block[18] & 8):
                                payload.extend(block)
                            if block[18] & 8 and (selected or block[17] == 0):
                                stream_data.extend(block)
                            if ((word >> 27) & 3) == 2 and block[17] == 0 and block[18] & 8:
                                bcd = lambda value: (value >> 4) * 10 + (value & 15)
                                physical = (bcd(block[12]) * 60 + bcd(block[13])) * 75 + bcd(block[14]) - 150
                                metadata.extend(struct.pack('<I', physical) + block[24:540])
                            if selected and block[18] & 4:
                                current = (block[16], block[17], block[19])
                                if block[19] & 0xEA:
                                    raise ValueError(f'Unsupported XA coding {disc}:{index}')
                                if tuple_ is not None and current != tuple_:
                                    raise ValueError(f'Changing XA tuple {disc}:{index}')
                                tuple_ = current
                                if first_audio is None:
                                    first_audio = sector - start
                                audio.extend(block)
                        entry.update(channel=channel, sectors=sectors, mode=(word >> 27) & 3,
                                     **publish(target, payload, verify, previous.get(target.relative_to(root).as_posix())))
                        if entry['mode'] == 3:
                            stream_file = folder / 'ACE_STREAM' / str(index)
                            entry['stream_data'] = dict(path=stream_file.relative_to(root).as_posix(),
                                                        **publish(stream_file, stream_data, verify))
                        if entry['mode'] == 2:
                            meta = folder / 'ACE_META' / str(index)
                            entry['metadata'] = dict(path=meta.relative_to(root).as_posix(), record_bytes=520,
                                                     **publish(meta, metadata, verify))
                        if audio:
                            audio_path = root / 'bin/MUSIC' / f'{disc}_{index}.XA' if index in music else folder / 'ACE_XA' / str(index)
                            entry['audio_policy'] = 'stream_music' if index in music else 'packaged_xa'
                            entry['xa'] = dict(path=audio_path.relative_to(root).as_posix(),
                                               tuple=tuple_, first_sector=first_audio,
                                               **publish(audio_path, audio, verify))
                            old_audio = previous_audio.get((disc, index))
                            if refresh and not verify and old_audio and old_audio['path'] != entry['xa']['path']:
                                old_path = root / old_audio['path']
                                if old_path.exists():
                                    if old_path.resolve().parent != (root / 'bin/MUSIC').resolve() or hashlib.sha256(old_path.read_bytes()).hexdigest() != old_audio['sha256'] or old_audio['sha256'] != entry['xa']['sha256']:
                                        raise ValueError('Modified obsolete MUSIC output')
                                    old_path.unlink()
                            # Main-image descriptor 0x800B22EC loops resource 0x826 after 360 physical sectors
                            if index == 38 and ((word >> 27) & 3) == 2:
                                tail = bytearray()
                                for sector in range(start + 360, end):
                                    block = data[sector * 2352:(sector + 1) * 2352]
                                    if block[17] == channel and block[18] & 4:
                                        tail.extend(block)
                                variant = root / 'bin/MUSIC' / f'{disc}_{index}_360.XA'
                                entry['xa_offsets'] = [dict(offset=360, path=variant.relative_to(root).as_posix(),
                                                            **publish(variant, tail, verify))]
                    entry['path'] = target.relative_to(root).as_posix()
            result['archives'].append(dict(kind=kind, source_sha256=identity,
                                           header_sha256=hashlib.sha256(header).hexdigest(), entries=entries))
        report['discs'].append(result)
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--project', type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument('--disc', type=int, choices=(1, 2), action='append')
    parser.add_argument('--verify', action='store_true')
    parser.add_argument('--refresh-derived', action='store_true', help='Replace only outputs still matching the previous extraction receipt')
    args = parser.parse_args()
    root = args.project.resolve()
    report = extract(root, args.disc or (1, 2), args.verify, args.refresh_derived)
    output = root / 'status/extracted-assets/extraction.json'
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps(dict(report=str(output), verified=args.verify,
                          resources=sum(e['present'] for d in report['discs'] for a in d['archives'] for e in a['entries']))))


if __name__ == '__main__':
    main()
