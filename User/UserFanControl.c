#include "UserFanControl.h"

static FanControlState _mode_state = manual_mode;

static int8 _temp_limit = 26;
static u8 _fan_state = 0;

void UserFanControl_Init(void)
{
    // 初始化所有子系统
    TimerInit();
    TaskManager_init();
    LED_Init();
    LED_Update(LED_STATE_STOP);
    Fan_Init();
    Key_Init();
    TM1650_Init(TM1650_BRIGHTNESS_8);

    // 绑定task更新
    Timer0_SetCallback(TaskUpdate);

    // 注册任务
    TaskManager_addTask(20, Key_Scan);
    TaskManager_addTask(1000, TM1650_Display);
    TaskManager_addTask(100, Handle_Key_Event);
    TaskManager_addTask(200, Led_Update);
    TaskManager_addTask(500, Display_Update);
}

void UserFanControl_Loop(void)
{
    TaskExecutor();
}

static void Display_Update(void)
{
    int16 temp;
    u8 w0, w1, w2, w3;
    u8 bit_mask = 0;

    temp = DS18B20_GetTemp();
    if (temp < 0) temp = -temp;

    w0 = temp % 10;
    w1 = (temp / 10) % 10;
    if (_mode_state == manual_mode)
    {
        w2 = 0;
        w3 = _fan_state;
    }else
    {
        w2 = _temp_limit % 10;
        w3 = (_temp_limit / 10) % 10;
    }

    TM1650_Set(w0, w1, w2, w3, bit_mask);
}

static void Led_Update(void)
{
    if (_mode_state == auto_mode)
    {
        LED_Update(LED_STATE_AUTO);
    }
    else if (_fan_state == 1)
    {
        LED_Update(LED_STATE_RUN);
    }else
    {
        LED_Update(LED_STATE_STOP);
    }
}

static void Handle_Key_Event(void)
{
    Key_res key_state = Key_GetPressed();
    if (key_state == NONE)
    {
        return;
    }else if (key_state == KEY1_ONLY)
    {
        if (_mode_state == manual_mode)
        {
            _fan_state = 1;
        }else if (_mode_state == auto_mode)
        {
            if (_temp_limit < 36) _temp_limit++;
        }
    }else if (key_state == KEY2_ONLY)
    {
        if (_mode_state == manual_mode)
        {
            _fan_state = 0;
        }else if (_mode_state == auto_mode)
        {
            if (_temp_limit > 16) _temp_limit--;
        }
        
    }else if (key_state == KEY1_AND_KEY2)
    {
        if (_mode_state == manual_mode)
        {
            _mode_state = auto_mode;
        }else if (_mode_state == auto_mode)
        {
            _mode_state = manual_mode;
        }
    }
}