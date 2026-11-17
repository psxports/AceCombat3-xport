#include "random.h"

uint32 random_seed;

uint32 game_random(void)
{
    uint32 seed = random_seed;
    uint32 result = (uint32)psyq_rand_next(&seed);
    random_seed = seed;
    return result;
}
