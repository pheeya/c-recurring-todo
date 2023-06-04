#include "Task.h"
#include <stdlib.h>
#include <string.h>
int td_selected = 0;
int td_num_daily_tasks = 0;
static Task *DailyTasks;
Task *ReadTasks(const char *_filePath, int *num)
{
    FILE *file = fopen(_filePath, "r");

    if (file == NULL)
    {
        return NULL;
    }

    char line[MAX_LINE_LENGTH];

    Task *tasks = NULL;

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL)
    {
        char delim[] = " ";
        char *token = strtok(line, delim);
        const char *description = token;

        token = strtok(NULL, delim);
        bool completed = *token == '1';

        AddTask(&tasks, num, description, completed);
    }

    return tasks;
}

Task *AddTask(Task **taskList, int *numTasks, const char *_description, bool _completed)
{
    // Allocate memory for the new task
    Task *newTask = (Task *)malloc(sizeof(Task));
    if (newTask == NULL)
    {
        // Error handling if memory allocation fails
        printf("Memory allocation failed.\n");
        return NULL;
    }

    // Set the fields of the new task
    strncpy(newTask->description, _description, sizeof(newTask->description));
    newTask->completed = _completed;

    // Increase the size of the task list
    *taskList = (Task *)realloc(*taskList, (*numTasks + 1) * sizeof(Task));
    if (*taskList == NULL)
    {
        // Error handling if memory reallocation fails
        printf("Memory reallocation failed.\n");
        free(newTask);
        return NULL;
    }

    // Add the new task to the task list
    (*taskList)[*numTasks] = *newTask;
    (*numTasks)++;
    return newTask;
}

void LoadTasks()
{
    DailyTasks = ReadTasks("daily.txt", &td_num_daily_tasks);
}
Task *GetDailyTasks()
{
    return DailyTasks;
}
void OnAppStart()
{
    LoadTasks();
}

void TaskStartEventListener(Event *_event)
{
    OnAppStart();
}
static bool canGoDown = true;
static bool canGoUp = true;

static void OnUpdate()
{
    if (GetKeyDown(TD_KEY_ARROW_DOWN) && canGoDown)
    {
        canGoDown = false;

        td_selected++;
        if (td_selected == td_num_daily_tasks)
            td_selected = 0;
        FRAME_DIRTY = true;
    }

    if (GetKeyDown(TD_KEY_ARROW_UP) && canGoUp)
    {
        canGoUp = false;

        td_selected--;
        if (td_selected < 0)
            td_selected = td_num_daily_tasks - 1;
        FRAME_DIRTY = true;
    }

    if (!GetKeyDown(TD_KEY_ARROW_DOWN))
    {
        canGoDown = true;
    }

    if (!GetKeyDown(TD_KEY_ARROW_UP))
    {
        canGoUp = true;
    }
}

void TaskUpdateEventListener(Event *_event)
{
    OnUpdate();
}
