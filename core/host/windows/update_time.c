#include "update_time.h"

#include <SDL3/SDL.h>
#include "game.h"

inline void update_time(void)
{
    static uint64_t last_clock_state = 0;
    uint64_t start_clock = SDL_GetTicksNS();
    game.delta_time_ms = SDL_NS_TO_MS((float)(start_clock - last_clock_state));
    if(game.delta_time_ms > 100.0f) game.delta_time_ms = 100.0f;
    last_clock_state = start_clock;
}
