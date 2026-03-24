
#include "WouoUI_init.h"
#include "EEPROM.h"
#include "../Wouo.h"
#include "WouoUI_menu_data.h"

extern UiContext ui;
extern eeprom_t eeprom;
extern ParamWindow win;
extern knob_t knob;
extern EEPROM_ EEPROM;
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;


extern uint8_t *buf_ptr;  // 指向屏幕缓冲的指针
extern uint16_t buf_len;  // 缓冲长度

// 在初始化EEPROM时，选择性初始化的默认设置
void ui_param_init()
{
    ui.param[DISP_BRI] = 255; // 屏幕亮度
    ui.param[TILE_ANI] = 30;  // 磁贴动画速度
    ui.param[LIST_ANI] = 60;  // 列表动画速度
    ui.param[WIN_ANI] = 25;   // 弹窗动画速度
    ui.param[SPOT_ANI] = 50;  // 聚光动画速度
    ui.param[TAG_ANI] = 60;   // 标签动画速度
    ui.param[FADE_ANI] = 30;  // 消失动画速度
    ui.param[BTN_SPT] = 25;   // 按键短按时长
    ui.param[BTN_LPT] = 150;  // 按键长按时长
    ui.param[TILE_UFD] = 1;   // 磁贴图标从头展开开关
    ui.param[LIST_UFD] = 1;   // 菜单列表从头展开开关
    ui.param[TILE_LOOP] = 0;  // 磁贴图标循环模式开关
    ui.param[LIST_LOOP] = 0;  // 菜单列表循环模式开关
    ui.param[WIN_BOK] = 0;    // 弹窗背景虚化开关
    ui.param[KNOB_DIR] = 0;   // 旋钮方向切换开关
    ui.param[DARK_MODE] = 1;  // 黑暗模式开关
}

// 列表类页面列表行数初始化，必须初始化的参数
void ui_init()
{
    ui.num[M_MAIN]    = main_menu_size;
    ui.num[M_EDITOR]  = editor_menu_size;
    ui.num[M_KNOB]    = knob_menu_size;
    ui.num[M_KRF]     = krf_menu_size;
    ui.num[M_KPF]     = kpf_menu_size;
    ui.num[M_VOLT]    = volt_menu_size;
    ui.num[M_SETTING] = setting_menu_size;
    ui.num[M_ABOUT]   = about_menu_size;
}



/********************************* 初始化数据处理函数 *******************************/

// 显示数值的初始化
void check_box_v_init(uint8_t *param)
{
    check_box.v = param;
}

// 多选框的初始化
void check_box_m_init(uint8_t *param)
{
    check_box.m = param;
}

// 单选框时的初始化
void check_box_s_init(uint8_t *param, uint8_t *param_p)
{
    check_box.s = param;
    check_box.s_p = param_p;
}

// 多选框处理函数
void check_box_m_select(uint8_t param)
{
    check_box.m[param] = !check_box.m[param];
    eeprom.change = true;
}

// 单选框处理函数
void check_box_s_select(uint8_t val, uint8_t pos)
{
    *check_box.s = val;
    *check_box.s_p = pos;
    eeprom.change = true;
}

// 弹窗数值初始化
void window_value_init(char title[], uint8_t select, uint8_t *value, uint8_t max, uint8_t min, uint8_t step, MENU *bg, uint8_t index)
{
    strcpy(win.title, title);
    win.select = select;
    win.value = value;
    win.max = max;
    win.min = min;
    win.step = step;
    win.bg = bg;
    win.index = index;
    ui.index = M_WINDOW;
    ui.state = S_WINDOW;
}

// EEPROM读数据，开机初始化时执行一遍
void eeprom_read_all_data()
{
    eeprom.address = EEPROM_CHECK;
    for (uint8_t i = 0; i < UI_PARAM; ++i)
        ui.param[i] = EEPROM.read(eeprom.address + i);
    eeprom.address += UI_PARAM;
    for (uint8_t i = 0; i < KNOB_PARAM; ++i)
        knob.param[i] = EEPROM.read(eeprom.address + i);
    eeprom.address += KNOB_PARAM;
}

// EEPROM写数据，回到睡眠时执行一遍
void eeprom_write_all_data()
{
    eeprom.address = 0;
    for (uint8_t i = 0; i < EEPROM_CHECK; ++i)
        EEPROM.write(eeprom.address + i, eeprom.check_param[i]);
    eeprom.address += EEPROM_CHECK;
    for (uint8_t i = 0; i < UI_PARAM; ++i)
        EEPROM.write(eeprom.address + i, ui.param[i]);
    eeprom.address += UI_PARAM;
    for (uint8_t i = 0; i < KNOB_PARAM; ++i)
        EEPROM.write(eeprom.address + i, knob.param[i]);
    eeprom.address += KNOB_PARAM;
}


// 开机检查是否已经修改过，没修改过则跳过读配置步骤，用默认设置
void eeprom_init()
{
    eeprom.check = 0;
    eeprom.address = 0;
    for (uint8_t i = 0; i < EEPROM_CHECK; ++i)
        if (EEPROM.read(eeprom.address + i) != eeprom.check_param[i])
            eeprom.check++;
    if (eeprom.check <= 1)
        eeprom_read_all_data(); // 允许一位误码
    else
        ui_param_init();
}



/********************************* 分页面初始化函数 ********************************/

// 进入磁贴类时的初始化
void tile_param_init()
{
    ui.init = false;
    tile.icon_x = 0;
    tile.icon_x_trg = TILE_ICON_S;
    tile.icon_y = -TILE_ICON_H;
    tile.icon_y_trg = 0;
    tile.indi_x = 0;
    tile.indi_x_trg = TILE_INDI_W;
    tile.title_y = tile.title_y_calc;
    tile.title_y_trg = tile.title_y_trg_calc;
}

// 进入睡眠时的初始化
void sleep_param_init()
{
    u8g2.setDrawColor(0);
    u8g2.drawBox(0, 0, DISP_W, DISP_H);
    u8g2.setPowerSave(1);
    ui.state = S_NONE;
    ui.sleep = true;
    if (eeprom.change)
    {
        eeprom_write_all_data();
        eeprom.change = false;
    }
}

// 旋钮设置页初始化
void knob_param_init() { check_box_v_init(knob.param); }

// 旋钮旋转页初始化
void krf_param_init() { check_box_s_init(&knob.param[KNOB_ROT], &knob.param[KNOB_ROT_P]); }

// 旋钮点按页初始化
void kpf_param_init() { check_box_s_init(&knob.param[KNOB_COD], &knob.param[KNOB_COD_P]); }

// 电压测量页初始化
void volt_param_init()
{
    volt.text_bg_r = 0;
    volt.text_bg_r_trg = VOLT_TEXT_BG_W;
}

// 设置页初始化
void setting_param_init()
{
    check_box_v_init(ui.param);
    check_box_m_init(ui.param);
}

// OLED初始化函数
void oled_init()
{
    u8g2.setBusClock(1000000); // 硬件IIC接口使用
    u8g2.begin();
    u8g2.setContrast(ui.param[DISP_BRI]);
    buf_ptr = u8g2.getBufferPtr();
    buf_len = 8 * u8g2.getBufferTileHeight() * u8g2.getBufferTileWidth();
}