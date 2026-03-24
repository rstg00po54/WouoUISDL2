#ifndef __WonoUI_Init__
#define __WonoUI_Init__
#include "Wouo.h"
void ui_param_init();
void check_box_v_init(uint8_t *param);
void check_box_m_init(uint8_t *param);
void check_box_s_init(uint8_t *param, uint8_t *param_p);
void check_box_m_select(uint8_t param);
void check_box_s_select(uint8_t val, uint8_t pos);
void window_value_init(char title[], uint8_t select, uint8_t *value, uint8_t max, uint8_t min, uint8_t step, MENU *bg, uint8_t index);
void eeprom_init();
void ui_init();
void tile_param_init();
void knob_param_init();
void krf_param_init();
void kpf_param_init();
void volt_param_init();
void setting_param_init();
void sleep_param_init();
void oled_init();
#endif