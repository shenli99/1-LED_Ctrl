#include "Task.h"

TaskManager _taskManager = {0};

void TaskManager_init(void)
{
    u16 i;
    _taskManager.size = 0;
    for (i = 0; i < MAX_TASK_NUM; i++)
    {
        _taskManager.tasks[i].count = 0;
        _taskManager.tasks[i].period = 0;
        _taskManager.tasks[i].state = TASK_IDLE;
        _taskManager.tasks[i].poll = NULL;
    }
}

int8 TaskManager_addTask(u16 period, TaskFunc poll)
{
    if (_taskManager.size < MAX_TASK_NUM || _taskManager.tasks[_taskManager.size].state == TASK_IDLE)
    {
        _taskManager.tasks[_taskManager.size].period = period;
        _taskManager.tasks[_taskManager.size].poll = poll;
        _taskManager.tasks[_taskManager.size].state = TASK_RUNNING;
        _taskManager.tasks[_taskManager.size].count = period;
        _taskManager.size++;
        return _taskManager.size - 1;
    } else {
        return -1;
    }
}

void TaskUpdate(void)
{
    u8 i;
    for (i = 0; i < 5; i++)
    {
        if (_taskManager.tasks[i].count > 0) _taskManager.tasks[i].count -=  _taskManager.tasks[i].state & 0x01;
    }
}

void TaskExecutor(void)
{
    u8 i;
    while (1)
    {
        for (i = 0; i < 5; i++)
        {
            if (_taskManager.tasks[i].state == TASK_RUNNING && _taskManager.tasks[i].count == 0 && i < 5)
            {
                _taskManager.tasks[i].poll();
                _taskManager.tasks[i].count = _taskManager.tasks[i].period;
            }
        }
    }
}