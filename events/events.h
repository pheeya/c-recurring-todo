#include <stdlib.h>

#ifndef EVENTS
#define EVENTS

enum EventType
{
    EventEmpty = 0,
    EventUpdate,
    EventKeyboard,
    EventMouse,
    EventPaint,
};

typedef struct
{
    enum EventType type;
    void *data;
} Event;

typedef void (*EventListener)(Event *_event);

typedef struct EventListenerNode
{
    EventListener listener;
    struct EventListenerNode *next;
    enum EventType type;
} EventListenerNode;

extern EventListenerNode *ApplicationEventListeners;

typedef struct
{
    Event *events;
    int capacity;
    int front;
    int rear;
    int count;
} EventQueue;

void EnqueueEvent(EventQueue *_queue, Event _event);

Event DequeueEvent(EventQueue *_queue);

void RegisterEventListener(EventListener _listener, enum EventType _type);

void DispatchEvent(Event *_event);

#endif
