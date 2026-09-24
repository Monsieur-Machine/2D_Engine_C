#include "update_time.h"

#include <time.h>
#include "game.h"

inline void update_time(void)
{
    static clock_t last_clock_state = 0;
    clock_t start_clock = clock();
    game.delta_time_ms = (float)(start_clock - last_clock_state) / (float)CLOCKS_PER_SEC;
    if(game.delta_time_ms > 100.0f) game.delta_time_ms = 100.0f;
    last_clock_state = start_clock;
}
