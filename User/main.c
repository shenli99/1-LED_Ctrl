#include "UserFanControl.h"

// 主函数
void main(void)
{
    UserFanControl_Init();

    while (1)
    {
        UserFanControl_Loop();
    }
}