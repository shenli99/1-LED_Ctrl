#include "common.h"

typedef enum {
    TASK_IDLE = 0,
    TASK_RUNNING = 1,
    TASK_PAUSE = 2
} TaskState;

typedef  void (*TaskFunc)(void);

typedef struct {
    u16 count;
    u16 period;
    TaskState state;
    TaskFunc poll;
} Task;

typedef struct {
    u16 size;
    Task tasks[MAX_TASK_NUM];
} TaskManager;

void TaskManager_init(void);
int8 TaskManager_addTask(u16 period, TaskFunc poll);
void TaskUpdate(void);
void TaskExecutor(void);