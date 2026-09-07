#include "event_bus.h"

#include <stdlib.h>
#include "game.h"
#include "linked_list.h"
#include "memory_alloc.h"
#include "errors.h"

// Arg : callback_t callback
inline int create_subscription(subscription_t *subscription, va_list args)
{
    subscription->callback = va_arg(args, callback_t);
    subscription->data = va_arg(args, void *);

    return EXIT_SUCCESS;
}

// Arg : size_t id
inline int create_topic(topic_t *topic, va_list args)
{
    topic->id = va_arg(args, size_t);

    linked_list_init(&topic->subscriptions, sizeof(subscription_t), (allocator_t)create_subscription, NULL);

    return EXIT_SUCCESS;
}

inline int destroy_topic(topic_t *topic)
{
    if(linked_list_exit(&topic->subscriptions)) return_failure_int;

    return EXIT_SUCCESS;
}

inline void event_bus_init(void)
{
    linked_list_init(&game.event_bus.topics, sizeof(topic_t), (allocator_t)create_topic, (deleter_t)destroy_topic);

    debug_printf("Initialized event bus.");
}

inline int event_bus_exit(void)
{
    if(linked_list_exit(&game.event_bus.topics)) return_failure_int;

    debug_printf("Exited event bus.");
    return EXIT_SUCCESS;
}

// Arg : size_t id
static inline bool condition_is_topic_id(elem_t *elem, void *arg)
{
    return ((topic_t *)elem->data)->id == *(size_t *)arg;
}

int event_bus_new_topic(size_t id)
{
    if(linked_list_get_if(&game.event_bus.topics, condition_is_topic_id, &id)) return EXIT_SUCCESS;

    elem_t *topic_elem = create_elem(&game.event_bus.topics, id);
    if(!topic_elem) return_failure_int;

    linked_list_push_back(&game.event_bus.topics, topic_elem);

    debug_printf("Created new topic with id : %lu", id);

    return EXIT_SUCCESS;
}

inline int event_bus_remove_topic(size_t id)
{
    if(linked_list_remove_if(&game.event_bus.topics, condition_is_topic_id, &id)) return_failure_int;

    debug_printf("Removed topic with id : %lu", id);

    return EXIT_SUCCESS;
}

// Args : va_list args
static int action_execute_callback(elem_t *elem, void *arg)
{
    subscription_t *subscription = elem->data;

    if(subscription->callback(arg, subscription->data)) return_failure_int;

    return EXIT_SUCCESS;
}

int event_bus_publish(size_t id, void *arg)
{
    elem_t *topic_elem = linked_list_get_if(&game.event_bus.topics, condition_is_topic_id, &id);
    if(!topic_elem) return_failure_int;

    topic_t *topic = topic_elem->data;

    if(linked_list_for_each(&topic->subscriptions, action_execute_callback, arg)) return_failure_int;

    debug_printf("Published event on topic with id : %lu", id);

    return EXIT_SUCCESS;
}

int event_bus_subscribe(size_t id, callback_t callback, void *data)
{
    elem_t *topic_elem = linked_list_get_if(&game.event_bus.topics, condition_is_topic_id, &id);
    if(!topic_elem)
    {
        error_printf("Topic with id : %lu doesn't exists.", id);
        return EXIT_FAILURE;
    }

    topic_t *topic = topic_elem->data;

    elem_t *subscription_elem = create_elem(&topic->subscriptions, callback, data);
    if(!subscription_elem) return_failure_int;

    linked_list_push_back(&topic->subscriptions, subscription_elem);

    debug_printf("Subscribed to topic with id : %lu", id);

    return EXIT_SUCCESS;
}

// Arg : callback_t callback
static inline bool condition_is_callback(elem_t *elem, void *arg)
{
    return ((subscription_t *)elem->data)->callback == *(callback_t *)arg;
}

int event_bus_unsubscribe(size_t id, callback_t callback)
{
    elem_t *topic_elem = linked_list_get_if(&game.event_bus.topics, condition_is_topic_id, &id);
    if(!topic_elem) return_failure_int;

    topic_t *topic = topic_elem->data;

    if(linked_list_remove_if(&topic->subscriptions, condition_is_callback, &callback)) return_failure_int;

    return EXIT_SUCCESS;
}
