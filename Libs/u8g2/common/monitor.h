/**
 * @file monitor.h
 *
 */

#ifndef MONITOR_H
#define MONITOR_H

#ifdef __cplusplus
extern "C"
{
#endif
// #include <SDL2/SDL.h>
#include <stdint.h>
    /*********************
     *      DEFINES
     *********************/

    /**********************
     *      TYPEDEFS
     **********************/
    typedef int32_t Sint32;
    typedef uint32_t Uint32;
    /**********************
     * GLOBAL PROTOTYPES
     **********************/
    void monitor_init(void);
    void monitor_flush(Sint32 x, Sint32 y, Uint32 h);
    void monitor_buffer_flush(uint8_t *buf);
    // void monitor_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
    // void monitor_flush2(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);

    /**********************
     *      MACROS
     **********************/

#define LV_HOR_RES_MAX (128)
#define LV_VER_RES_MAX (128)

#endif /* USE_MONITOR */

#ifdef __cplusplus
} /* extern "C" */
#endif
