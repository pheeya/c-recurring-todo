#include "Task.h"
#include <stdlib.h>
#include <string.h>
Task *GetTasks(const char *_filePath, int *num)
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
