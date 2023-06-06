#include "Task.h"
#include <stdlib.h>
#include <string.h>
int td_selected = 0;
int td_num_daily_tasks = 0;
static Task *DailyTasks;

static int last_updated = 0;
static void ResetIfRequired()
{
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    int day = timeinfo->tm_wday;
    if (last_updated != day)
    {
        FILE *file = fopen("last-updated.txt", "w");
        FILE *tmp = fopen("tmp.tmp", "w");
        char dayNum[25];

        sprintf(dayNum, "%d", day);
        fputs(dayNum, tmp);

        fclose(file);
        fclose(tmp);

        remove("last-updated.txt");
        rename("tmp.tmp", "last-updated.txt");

        last_updated = day;

        for (int i = 0; i < td_num_daily_tasks; i++)
        {
            OverrideDailyTask(i, 0);
        }
    }
}

static void CheckLastUpdated()
{
    FILE *file = fopen("last-updated.txt", "r");

    if (file == NULL)
    {
        return;
    }

    char line[10];
    fgets(line, 10, file);
    last_updated = atoi(line);

    fclose(file);
}

Task *ReadTasks(const char *_filePath, int *num)
{
    FILE *file = fopen(_filePath, "r");

    if (file == NULL)
    {
        return NULL;
    }

    char line[MAX_LINE_LENGTH];

    Task *tasks = NULL;

    char delim[] = " ";
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL)
    {
        char *token = strtok(line, delim);
        char desc[MAX_LINE_LENGTH] = "";
        const char *val;

        while (token != NULL)
        {
            val = token;
            if (val != "0" && val != "1")
            {
                strcat(desc, token);
                strcat(desc, " ");
            }

            token = strtok(NULL, delim);
        }
        bool completed = *val == '1';

        AddTask(&tasks, num, desc, completed);
    }

    fclose(file);

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
    CheckLastUpdated();
    LoadTasks();
}

void TaskStartEventListener(Event *_event)
{
    OnAppStart();
}
static bool canGoDown = true;
static bool canGoUp = true;
static bool canToggleStatus = true;

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

    if (GetKeyDown(TD_KEY_SPACE) && canToggleStatus)
    {
        canToggleStatus = false;
        ToggleDailyTask(td_selected);
    }

    if (!GetKeyDown(TD_KEY_ARROW_DOWN))
    {
        canGoDown = true;
    }

    if (!GetKeyDown(TD_KEY_ARROW_UP))
    {
        canGoUp = true;
    }

    if (!GetKeyDown(TD_KEY_SPACE))
    {
        canToggleStatus = true;
    }
}

void TaskUpdateEventListener(Event *_event)
{
    OnUpdate();
    ResetIfRequired();
}

void ToggleDailyTask(int _index)
{
    DailyTasks[_index].completed = !DailyTasks[td_selected].completed;
    FRAME_DIRTY = true;
    UpdateTaskInFile("Daily.txt", td_selected, &DailyTasks[td_selected]);
}

void OverrideDailyTask(int _index, int _completed)
{
    DailyTasks[_index].completed = _completed;
    FRAME_DIRTY = true;
    UpdateTaskInFile("Daily.txt", td_selected, &DailyTasks[td_selected]);
}

void UpdateTaskInFile(const char *_filePath, int _taskNum, Task *_task)
{
    FILE *file = fopen(_filePath, "r");
    FILE *tmp = fopen("tmp.tmp", "w");
    if (file == NULL)
    {
        return;
    }

    char line[MAX_LINE_LENGTH];

    Task *tasks = NULL;
    int lineNum = 0;
    char newLine[MAX_LINE_LENGTH];

    strcpy(newLine, _task->description);
    strcat(newLine, " ");
    strcat(newLine, _task->completed ? "1" : "0");
    strcat(newLine, "\n");

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL)
    {
        if (lineNum == _taskNum)
        {
            fputs(newLine, tmp);
        }
        else
        {
            fputs(line, tmp);
        }

        lineNum++;
    }

    fclose(tmp);
    fclose(file);

    remove(_filePath);

    rename("tmp.tmp", _filePath);
}
