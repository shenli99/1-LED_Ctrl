#include "common.h"

typedef enum {
    TASK_IDLE = 0,
    TASK_RUNNING = 1,
    TASK_PAUSE = 2
} TaskState;

typedef  TaskState (*TaskFunc)(void);

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