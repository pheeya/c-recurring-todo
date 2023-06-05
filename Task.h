#ifndef TASK
#define TASK

#include "events/keyboard/keyboardEvent.h"
#include <stdbool.h>
#include <stdio.h>
#include "events/events.h"
#include <stdio.h>
#include "renderer.h"
#define MAX_LINE_LENGTH 256
extern int td_selected;
extern int td_num_daily_tasks;


typedef struct Task
{
    char description[MAX_LINE_LENGTH];
    bool completed;
} Task;

#endif

Task *ReadTasks(const char *_filePath, int *num);

Task *AddTask(Task **_list, int *numTasks, const char *_description, bool _completed);

static Task* DailyTasks;

void LoadTasks();

void OnAppStart();


void TaskStartEventListener(Event* _event);

void TaskUpdateEventListener(Event* _event);

static void OnUpdate();

Task* GetDailyTasks();



void ToggleTaskStatus();


void UpdateTaskInFile(const char* _filePath, int _taskNum, Task* _task);