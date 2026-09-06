#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <stdarg.h>
#include "linked_list.h"

typedef int (*callback_t)(void *event_arg, void *subscribtion_arg);

typedef struct subscription_t {
    callback_t callback;
    void *data;
} subscription_t;

int create_subscription(subscription_t *subscription, va_list args);

typedef struct topic_t {
    size_t id;
    linked_list_t subscriptions;
} topic_t;

int create_topic(topic_t *topic, va_list args);
int destroy_topic(topic_t *topic);

typedef struct event_bus_t {
    linked_list_t topics;
} event_bus_t;

void event_bus_init(void);
int event_bus_exit(void);

int event_bus_new_topic(size_t id);
int event_bus_remove_topic(size_t id);

int event_bus_publish(size_t id, void *arg);
int event_bus_subscribe(size_t id, callback_t callback, void *data);
int event_bus_unsubscribe(size_t id, callback_t callback);

#endif // EVENT_BUS_H