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
void tile_show(struct MENU arr_1[], const uint8_t icon_pic[][120]);
void tile_rotate_switch();

#endif /* WOUOUI_128_64_H */