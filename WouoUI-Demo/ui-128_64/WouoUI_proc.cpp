#include "WouoUI_proc.h"
#include "../Wouo.h"
#include "WouoUI-128_64.h"

extern M_SELECT main_menu[];
extern InputContext btn;
extern UiContext ui;
extern ParamWindow win;
extern eeprom_t eeprom;
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


// // 弹窗通用处理函数
// void window_proc()
// {
//     window_show();
//     if (win.y == WIN_Y_TRG)
//         ui.index = win.index;
//     if (btn.pressed && win.y == win.y_trg && win.y != WIN_Y_TRG)
//     {
//         btn.pressed = false;
//         switch (btn.id)
//         {
//         case BTN_ID_CW:
//             if (*win.value < win.max)
//                 *win.value += win.step;
//             eeprom.change = true;
//             break;
//         case BTN_ID_CC:
//             if (*win.value > win.min)
//                 *win.value -= win.step;
//             eeprom.change = true;
//             break;
//         case BTN_ID_SP:
//         case BTN_ID_LP:
//             win.y_trg = WIN_Y_TRG;
//             break;
//         }
//     }
// }