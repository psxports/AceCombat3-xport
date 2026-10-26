PROJECT_ROOT := ../../..
SHORT_NAME := AC3
FULL_NAME := Ace Combat 3
WEB_EXCLUDE_FILES := \
    DATA/1/ACE.BPB DATA/2/ACE.BPB \
    DATA/1/ACE.SPB DATA/2/ACE.SPB \
    DATA/1/ACE_SPB/* DATA/2/ACE_SPB/* \
    DATA/1/SLPS_020.20 DATA/2/SLPS_020.21 \
    DATA/1/SYSTEM.CNF DATA/2/SYSTEM.CNF
include extracted_exclusions.mk
APP_SOURCES := $(wildcard ../../../src/batch_*.c) ../../../src/game_main.c ../../../src/game_sdk.c ../../../src/game_data.c ../../../src/game_dispatch.c
APP_DEFINES := -DXPORT_NATIVE_CD=1 -DXPORT_NATIVE_CALLBACK_BOOTSTRAP=1 -DMDEC_CUSTOM=1 -DXPORT_EXTRACTED_ASSETS=1 -DXPORT_PAD_HACK=0 -DFIELD_RATE=60 -DWND_WIDTH=960 -DWND_HEIGHT=720
APP_DEFINES += -DWND_TITLE=\"AC3\"
ifeq ($(CONFIG),Release)
APP_DEFINES += -DXPORT_DISABLE_FUNCTION_MARKERS
endif

include ../../../../xport/src/platform/web/Makefile

LINK_FLAGS += -sASYNCIFY_STACK_SIZE=65536 -sSTACK_OVERFLOW_CHECK=2
