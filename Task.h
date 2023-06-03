#ifndef TASK
#define TASK

#include <stdbool.h>
#include <stdio.h>
#define MAX_LINE_LENGTH 256
typedef struct Task
{
    char description[MAX_LINE_LENGTH];
    bool completed;
} Task;



#endif



Task* GetTasks(const char* _filePath, int *num);

Task* AddTask(Task** _list, int* numTasks, const char* _description, bool _completed);