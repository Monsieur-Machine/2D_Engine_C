#include "inputs.h"

#include <gint/keyboard.h>

inline int poll_event(input_event_t *event)
{
    key_event_t gint_event = pollevent();

    if(gint_event.type = KEYEV_NONE)
        return 0;

    event->type = gint_event.type;
    event->key = gint_event.key;

    return 1;
}
