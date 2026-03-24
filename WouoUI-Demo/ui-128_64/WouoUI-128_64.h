#ifndef WOUOUI_128_64_H
#define WOUOUI_128_64_H
#include "../Wouo.h"
// 按键ID
#define BTN_ID_CC 0 // 逆时针旋转
#define BTN_ID_CW 1 // 顺时针旋转
#define BTN_ID_SP 2 // 短按
#define BTN_ID_LP 3 // 长按
void setup();
void loop();
void window_show();
void tile_show(struct MENU arr_1[], const uint8_t icon_pic[][120]);
void tile_rotate_switch();
void btn_scan();
void list_show(struct MENU arr[], uint8_t ui_index);
void list_rotate_switch();
void check_box_s_init(uint8_t *param, uint8_t *param_p);
void check_box_s_select(uint8_t val, uint8_t pos);
void volt_show();
void window_value_init(char title[], uint8_t select, uint8_t *value, uint8_t max, uint8_t min, uint8_t step, MENU *bg, uint8_t index);
void fade();
void check_box_m_select(uint8_t param);

void window_param_init();
void layer_init_in();
void layer_init_out();
#endif /* WOUOUI_128_64_H */