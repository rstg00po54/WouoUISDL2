#include "WouoUI_proc.h"
#include "../Wouo.h"
#include "WouoUI-128_64.h"
#include "USBComposite.h"
#include "WouoUI_menu_data.h"

extern M_SELECT main_menu[];
extern InputContext btn;
extern UiContext ui;
extern ParamWindow win;
extern eeprom_t eeprom;
extern knob_t knob;
extern HIDConsumer Consumer;
extern HIDKeyboard Keyboard;
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
// 主菜单处理函数，磁贴类模板
void main_proc()
{
    tile_show(main_menu, main_icon_pic);
    if (btn.pressed)
    {
        btn.pressed = false;
        switch (btn.id)
        {
        case BTN_ID_CW:
        case BTN_ID_CC:
            tile_rotate_switch();
            break;
        case BTN_ID_SP:
            switch (ui.select[ui.layer])
            {

            case 0:
                ui.index = M_SLEEP;
                ui.state = S_LAYER_OUT;
                break;
            case 1:
                ui.index = M_EDITOR;
                ui.state = S_LAYER_IN;
                break;
            case 2:
                ui.index = M_VOLT;
                ui.state = S_LAYER_IN;
                break;
            case 3:
                ui.index = M_SETTING;
                ui.state = S_LAYER_IN;
                break;
            }
        }
        if (!tile.select_flag && ui.init)
        {
            tile.indi_x = 0;
            tile.title_y = tile.title_y_calc;
        }
    }
}


// 弹窗通用处理函数
void window_proc()
{
    window_show();
    if (win.y == WIN_Y_TRG)
        ui.index = win.index;
    if (btn.pressed && win.y == win.y_trg && win.y != WIN_Y_TRG)
    {
        btn.pressed = false;
        switch (btn.id)
        {
        case BTN_ID_CW:
            if (*win.value < win.max)
                *win.value += win.step;
            eeprom.change = true;
            break;
        case BTN_ID_CC:
            if (*win.value > win.min)
                *win.value -= win.step;
            eeprom.change = true;
            break;
        case BTN_ID_SP:
        case BTN_ID_LP:
            win.y_trg = WIN_Y_TRG;
            break;
        }
    }
}


/********************************** 分页面处理函数 **********************************/

// 睡眠页面处理函数
void sleep_proc()
{
    while (ui.sleep)
    {
        // 睡眠时循环执行的函数
        SDL_Delay(5);
        // 睡眠时需要扫描旋钮才能退出睡眠
        btn_scan();

        // 当旋钮有动作时
        if (btn.pressed)
        {
            btn.pressed = false;
            switch (btn.id)
            {

            // 睡眠时顺时针旋转执行的函数
            case BTN_ID_CW:
                switch (knob.param[KNOB_ROT])
                {
                case KNOB_ROT_VOL:
                    Consumer.press(HIDConsumer::VOLUME_UP);
                    Consumer.release();
                    break;
                case KNOB_ROT_BRI:
                    Consumer.press(HIDConsumer::BRIGHTNESS_UP);
                    Consumer.release();
                    break;
                }
                break;

            // 睡眠时逆时针旋转执行的函数
            case BTN_ID_CC:
                switch (knob.param[KNOB_ROT])
                {
                case KNOB_ROT_VOL:
                    Consumer.press(HIDConsumer::VOLUME_DOWN);
                    Consumer.release();
                    break;
                case KNOB_ROT_BRI:
                    Consumer.press(HIDConsumer::BRIGHTNESS_DOWN);
                    Consumer.release();
                    break;
                }
                break;

            // 睡眠时短按执行的函数
            case BTN_ID_SP:
                Keyboard.press(knob.param[KNOB_COD]);
                Keyboard.release(knob.param[KNOB_COD]);
                break;

            // 睡眠时长按执行的函数
            case BTN_ID_LP:
                ui.index = M_MAIN;
                ui.state = S_LAYER_IN;
                u8g2.setPowerSave(0);
                ui.sleep = false;
                break;
            }
        }
    }
}


// 编辑器菜单处理函数
void editor_proc()
{
    list_show(editor_menu, M_EDITOR);
    if (btn.pressed)
    {
        btn.pressed = false;
        switch (btn.id)
        {
        case BTN_ID_CW:
        case BTN_ID_CC:
            list_rotate_switch();
            break;
        case BTN_ID_LP:
            ui.select[ui.layer] = 0;
        case BTN_ID_SP:
            switch (ui.select[ui.layer])
            {

            case 0:
                ui.index = M_MAIN;
                ui.state = S_LAYER_OUT;
                break;
            case 11:
                ui.index = M_KNOB;
                ui.state = S_LAYER_IN;
                break;
            }
        }
    }
}

// 旋钮编辑菜单处理函数
void knob_proc()
{
    list_show(knob_menu, M_KNOB);
    if (btn.pressed)
    {
        btn.pressed = false;
        switch (btn.id)
        {
        case BTN_ID_CW:
        case BTN_ID_CC:
            list_rotate_switch();
            break;
        case BTN_ID_LP:
            ui.select[ui.layer] = 0;
        case BTN_ID_SP:
            switch (ui.select[ui.layer])
            {

            case 0:
                ui.index = M_EDITOR;
                ui.state = S_LAYER_OUT;
                break;
            case 1:
                ui.index = M_KRF;
                ui.state = S_LAYER_IN;
                check_box_s_init(&knob.param[KNOB_ROT], &knob.param[KNOB_ROT_P]);
                break;
            case 2:
                ui.index = M_KPF;
                ui.state = S_LAYER_IN;
                check_box_s_init(&knob.param[KNOB_COD], &knob.param[KNOB_COD_P]);
                break;
            }
        }
    }
}

// 旋钮旋转功能菜单处理函数
void krf_proc()
{
    list_show(krf_menu, M_KRF);
    if (btn.pressed)
    {
        btn.pressed = false;
        switch (btn.id)
        {
        case BTN_ID_CW:
        case BTN_ID_CC:
            list_rotate_switch();
            break;
        case BTN_ID_LP:
            ui.select[ui.layer] = 0;
        case BTN_ID_SP:
            switch (ui.select[ui.layer])
            {

            case 0:
                ui.index = M_KNOB;
                ui.state = S_LAYER_OUT;
                break;
            case 1:
                break;
            case 2:
                check_box_s_select(KNOB_DISABLE, ui.select[ui.layer]);
                break;
            case 3:
                break;
            case 4:
                check_box_s_select(KNOB_ROT_VOL, ui.select[ui.layer]);
                break;
            case 5:
                check_box_s_select(KNOB_ROT_BRI, ui.select[ui.layer]);
                break;
            case 6:
                break;
            }
        }
    }
}

// 旋钮点按功能菜单处理函数
void kpf_proc()
{
    list_show(kpf_menu, M_KPF);
    if (btn.pressed)
    {
        btn.pressed = false;
        switch (btn.id)
        {
        case BTN_ID_CW:
        case BTN_ID_CC:
            list_rotate_switch();
            break;
        case BTN_ID_LP:
            ui.select[ui.layer] = 0;
        case BTN_ID_SP:
            switch (ui.select[ui.layer])
            {

            case 0:
                ui.index = M_KNOB;
                ui.state = S_LAYER_OUT;
                break;
            case 1:
                break;
            case 2:
                check_box_s_select(KNOB_DISABLE, ui.select[ui.layer]);
                break;
            case 3:
                break;
            case 4:
                check_box_s_select('A', ui.select[ui.layer]);
                break;
            case 5:
                check_box_s_select('B', ui.select[ui.layer]);
                break;
            case 6:
                check_box_s_select('C', ui.select[ui.layer]);
                break;
            case 7:
                check_box_s_select('D', ui.select[ui.layer]);
                break;
            case 8:
                check_box_s_select('E', ui.select[ui.layer]);
                break;
            case 9:
                check_box_s_select('F', ui.select[ui.layer]);
                break;
            case 10:
                check_box_s_select('G', ui.select[ui.layer]);
                break;
            case 11:
                check_box_s_select('H', ui.select[ui.layer]);
                break;
            case 12:
                check_box_s_select('I', ui.select[ui.layer]);
                break;
            case 13:
                check_box_s_select('J', ui.select[ui.layer]);
                break;
            case 14:
                check_box_s_select('K', ui.select[ui.layer]);
                break;
            case 15:
                check_box_s_select('L', ui.select[ui.layer]);
                break;
            case 16:
                check_box_s_select('M', ui.select[ui.layer]);
                break;
            case 17:
                check_box_s_select('N', ui.select[ui.layer]);
                break;
            case 18:
                check_box_s_select('O', ui.select[ui.layer]);
                break;
            case 19:
                check_box_s_select('P', ui.select[ui.layer]);
                break;
            case 20:
                check_box_s_select('Q', ui.select[ui.layer]);
                break;
            case 21:
                check_box_s_select('R', ui.select[ui.layer]);
                break;
            case 22:
                check_box_s_select('S', ui.select[ui.layer]);
                break;
            case 23:
                check_box_s_select('T', ui.select[ui.layer]);
                break;
            case 24:
                check_box_s_select('U', ui.select[ui.layer]);
                break;
            case 25:
                check_box_s_select('V', ui.select[ui.layer]);
                break;
            case 26:
                check_box_s_select('W', ui.select[ui.layer]);
                break;
            case 27:
                check_box_s_select('X', ui.select[ui.layer]);
                break;
            case 28:
                check_box_s_select('Y', ui.select[ui.layer]);
                break;
            case 29:
                check_box_s_select('Z', ui.select[ui.layer]);
                break;
            case 30:
                break;
            case 31:
                check_box_s_select('0', ui.select[ui.layer]);
                break;
            case 32:
                check_box_s_select('1', ui.select[ui.layer]);
                break;
            case 33:
                check_box_s_select('2', ui.select[ui.layer]);
                break;
            case 34:
                check_box_s_select('3', ui.select[ui.layer]);
                break;
            case 35:
                check_box_s_select('4', ui.select[ui.layer]);
                break;
            case 36:
                check_box_s_select('5', ui.select[ui.layer]);
                break;
            case 37:
                check_box_s_select('6', ui.select[ui.layer]);
                break;
            case 38:
                check_box_s_select('7', ui.select[ui.layer]);
                break;
            case 39:
                check_box_s_select('8', ui.select[ui.layer]);
                break;
            case 40:
                check_box_s_select('9', ui.select[ui.layer]);
                break;
            case 41:
                break;
            case 42:
                check_box_s_select(KEY_ESC, ui.select[ui.layer]);
                break;
            case 43:
                check_box_s_select(KEY_F1, ui.select[ui.layer]);
                break;
            case 44:
                check_box_s_select(KEY_F2, ui.select[ui.layer]);
                break;
            case 45:
                check_box_s_select(KEY_F3, ui.select[ui.layer]);
                break;
            case 46:
                check_box_s_select(KEY_F4, ui.select[ui.layer]);
                break;
            case 47:
                check_box_s_select(KEY_F5, ui.select[ui.layer]);
                break;
            case 48:
                check_box_s_select(KEY_F6, ui.select[ui.layer]);
                break;
            case 49:
                check_box_s_select(KEY_F7, ui.select[ui.layer]);
                break;
            case 50:
                check_box_s_select(KEY_F8, ui.select[ui.layer]);
                break;
            case 51:
                check_box_s_select(KEY_F9, ui.select[ui.layer]);
                break;
            case 52:
                check_box_s_select(KEY_F10, ui.select[ui.layer]);
                break;
            case 53:
                check_box_s_select(KEY_F11, ui.select[ui.layer]);
                break;
            case 54:
                check_box_s_select(KEY_F12, ui.select[ui.layer]);
                break;
            case 55:
                break;
            case 56:
                check_box_s_select(KEY_LEFT_CTRL, ui.select[ui.layer]);
                break;
            case 57:
                check_box_s_select(KEY_LEFT_SHIFT, ui.select[ui.layer]);
                break;
            case 58:
                check_box_s_select(KEY_LEFT_ALT, ui.select[ui.layer]);
                break;
            case 59:
                check_box_s_select(KEY_LEFT_GUI, ui.select[ui.layer]);
                break;
            case 60:
                check_box_s_select(KEY_RIGHT_CTRL, ui.select[ui.layer]);
                break;
            case 61:
                check_box_s_select(KEY_RIGHT_SHIFT, ui.select[ui.layer]);
                break;
            case 62:
                check_box_s_select(KEY_RIGHT_ALT, ui.select[ui.layer]);
                break;
            case 63:
                check_box_s_select(KEY_RIGHT_GUI, ui.select[ui.layer]);
                break;
            case 64:
                break;
            case 65:
                check_box_s_select(KEY_CAPS_LOCK, ui.select[ui.layer]);
                break;
            case 66:
                check_box_s_select(KEY_BACKSPACE, ui.select[ui.layer]);
                break;
            case 67:
                check_box_s_select(KEY_RETURN, ui.select[ui.layer]);
                break;
            case 68:
                check_box_s_select(KEY_INSERT, ui.select[ui.layer]);
                break;
            case 69:
                check_box_s_select(KEY_DELETE, ui.select[ui.layer]);
                break;
            case 70:
                check_box_s_select(KEY_TAB, ui.select[ui.layer]);
                break;
            case 71:
                break;
            case 72:
                check_box_s_select(KEY_HOME, ui.select[ui.layer]);
                break;
            case 73:
                check_box_s_select(KEY_END, ui.select[ui.layer]);
                break;
            case 74:
                check_box_s_select(KEY_PAGE_UP, ui.select[ui.layer]);
                break;
            case 75:
                check_box_s_select(KEY_PAGE_DOWN, ui.select[ui.layer]);
                break;
            case 76:
                break;
            case 77:
                check_box_s_select(KEY_UP_ARROW, ui.select[ui.layer]);
                break;
            case 78:
                check_box_s_select(KEY_DOWN_ARROW, ui.select[ui.layer]);
                break;
            case 79:
                check_box_s_select(KEY_LEFT_ARROW, ui.select[ui.layer]);
                break;
            case 80:
                check_box_s_select(KEY_RIGHT_ARROW, ui.select[ui.layer]);
                break;
            case 81:
                break;
            }
        }
    }
}

// 电压测量页处理函数
void volt_proc()
{
    volt_show();
    if (btn.pressed)
    {
        btn.pressed = false;
        switch (btn.id)
        {
        case BTN_ID_CW:
        case BTN_ID_CC:
            list_rotate_switch();
            break;

        case BTN_ID_SP:
        case BTN_ID_LP:
            ui.index = M_MAIN;
            ui.state = S_LAYER_OUT;
            break;
        }
    }
}

// 设置菜单处理函数，多选框列表类模板，弹窗模板
void setting_proc()
{
    list_show(setting_menu, M_SETTING);
    if (btn.pressed)
    {
        btn.pressed = false;
        switch (btn.id)
        {
        case BTN_ID_CW:
        case BTN_ID_CC:
            list_rotate_switch();
            break;
        case BTN_ID_LP:
            ui.select[ui.layer] = 0;
        case BTN_ID_SP:
            switch (ui.select[ui.layer])
            {

            // 返回更浅层级，长按被当作选择这一项，也是执行这一行
            case 0:
                ui.index = M_MAIN;
                ui.state = S_LAYER_OUT;
                break;

            // 弹出窗口，参数初始化：标题，参数名，参数值，最大值，最小值，步长，背景列表名，背景列表标签
            case 1:
                window_value_init("Disp Bri", DISP_BRI, &ui.param[DISP_BRI], 255, 0, 5, setting_menu, M_SETTING);
                break;
            case 2:
                window_value_init("Tile Ani", TILE_ANI, &ui.param[TILE_ANI], 100, 10, 1, setting_menu, M_SETTING);
                break;
            case 3:
                window_value_init("List Ani", LIST_ANI, &ui.param[LIST_ANI], 100, 10, 1, setting_menu, M_SETTING);
                break;
            case 4:
                window_value_init("Win Ani", WIN_ANI, &ui.param[WIN_ANI], 100, 10, 1, setting_menu, M_SETTING);
                break;
            case 5:
                window_value_init("Spot Ani", SPOT_ANI, &ui.param[SPOT_ANI], 100, 10, 1, setting_menu, M_SETTING);
                break;
            case 6:
                window_value_init("Tag Ani", TAG_ANI, &ui.param[TAG_ANI], 100, 10, 1, setting_menu, M_SETTING);
                break;
            case 7:
                window_value_init("Fade Ani", FADE_ANI, &ui.param[FADE_ANI], 255, 0, 1, setting_menu, M_SETTING);
                break;
            case 8:
                window_value_init("Btn SPT", BTN_SPT, &ui.param[BTN_SPT], 255, 0, 1, setting_menu, M_SETTING);
                break;
            case 9:
                window_value_init("Btn LPT", BTN_LPT, &ui.param[BTN_LPT], 255, 0, 1, setting_menu, M_SETTING);
                break;

            // 多选框
            case 10:
                check_box_m_select(TILE_UFD);
                break;
            case 11:
                check_box_m_select(LIST_UFD);
                break;
            case 12:
                check_box_m_select(TILE_LOOP);
                break;
            case 13:
                check_box_m_select(LIST_LOOP);
                break;
            case 14:
                check_box_m_select(WIN_BOK);
                break;
            case 15:
                check_box_m_select(KNOB_DIR);
                break;
            case 16:
                check_box_m_select(DARK_MODE);
                break;

            // 关于本机
            case 17:
                ui.index = M_ABOUT;
                ui.state = S_LAYER_IN;
                break;
            }
        }
    }
}

// 关于本机页
void about_proc()
{
    list_show(about_menu, M_ABOUT);
    if (btn.pressed)
    {
        btn.pressed = false;
        switch (btn.id)
        {
        case BTN_ID_CW:
        case BTN_ID_CC:
            list_rotate_switch();
            break;
        case BTN_ID_LP:
            ui.select[ui.layer] = 0;
        case BTN_ID_SP:
            switch (ui.select[ui.layer])
            {

            case 0:
                ui.index = M_SETTING;
                ui.state = S_LAYER_OUT;
                break;
            }
        }
    }
}

// 总的UI进程
void ui_proc()
{
    u8g2.sendBuffer();
    switch (ui.state)
    {
    case S_FADE:
        fade();
        break; // 转场动画
    case S_WINDOW:
        window_param_init();
        break; // 弹窗初始化
    case S_LAYER_IN:
        layer_init_in();
        break; // 层级初始化
    case S_LAYER_OUT:
        layer_init_out();
        break; // 层级初始化

    case S_NONE:
        u8g2.clearBuffer();
        switch (ui.index) // 直接选择页面
        {
        case M_WINDOW:
            window_proc();
            break;
        case M_SLEEP:
            sleep_proc();
            break;
        case M_MAIN:
            main_proc();
            break;
        case M_EDITOR:
            editor_proc();
            break;
        case M_KNOB:
            knob_proc();
            break;
        case M_KRF:
            krf_proc();
            break;
        case M_KPF:
            kpf_proc();
            break;
        case M_VOLT:
            volt_proc();
            break;
        case M_SETTING:
            setting_proc();
            break;
        case M_ABOUT:
            about_proc();
            break;
        }
    }
}