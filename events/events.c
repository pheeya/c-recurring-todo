#include <stdlib.h>
#include "events.h"
#include <stdio.h>

EventListenerNode *ApplicationEventListeners;

void EnqueueEvent(EventQueue *_queue, Event _event)
{

    if (_queue->count == _queue->capacity)
    {
        _queue->capacity += 5;
    }

    _queue->events[_queue->rear] = _event;
    _queue->rear = (_queue->rear + 1) % _queue->capacity;
    _queue->count++;
}

Event DequeueEvent(EventQueue *_queue)
{

    if (_queue->count == 0)
    {
        Event emptyEvent;
        emptyEvent.type = EventEmpty;
        emptyEvent.data = NULL;
        return emptyEvent;
    }

    Event event = _queue->events[_queue->front];

    _queue->front = (_queue->front + 1) % _queue->capacity;
    _queue->count--;

    return event;
}

void RegisterEventListener(EventListener _listener, enum EventType _type)
{
    EventListenerNode *node = malloc(sizeof(EventListenerNode));
    node->listener = _listener;
    node->type = _type;
    node->next = ApplicationEventListeners;
    ApplicationEventListeners = node;
}

void DispatchEvent(Event *_event)
{
    EventListenerNode *currentNode = ApplicationEventListeners;

    while (currentNode != NULL)
    {
        if (currentNode->type == _event->type)
        {
            currentNode->listener(_event);
        }
        currentNode = currentNode->next;
    }
}
