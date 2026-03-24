/**
 * @file monitor.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "monitor.h"

// #define USE_MOUSE       1
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#define USE_KEYBOARD 1
// #include "../indev/mouse.h"
// #include "../indev/keyboard.h"
// #include "../indev/mousewheel.h"
/* Maximal horizontal and vertical resolution to support by the library.*/
#define LV_HOR_RES_MAX          (128)
#define LV_VER_RES_MAX          (64)

/*********************
 *      DEFINES
 *********************/
#define SDL_REFR_PERIOD 50 /*ms*/

#ifndef MONITOR_ZOOM
#define MONITOR_ZOOM 1
#endif

#ifndef MONITOR_HOR_RES
#define MONITOR_HOR_RES LV_HOR_RES_MAX
#endif

#ifndef MONITOR_VER_RES
#define MONITOR_VER_RES LV_VER_RES_MAX
#endif

#if defined(__APPLE__) && defined(TARGET_OS_MAC)
#if __APPLE__ && TARGET_OS_MAC
#define MONITOR_APPLE
#endif
#endif

#if defined(__EMSCRIPTEN__)
#define MONITOR_EMSCRIPTEN
#endif

/**********************
 *      TYPEDEFS
 **********************/
typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    volatile bool sdl_refr_qry;
#if MONITOR_DOUBLE_BUFFERED
    uint32_t *tft_fb_act;
#else
    uint32_t tft_fb[LV_HOR_RES_MAX * LV_VER_RES_MAX];
#endif
} monitor_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static int monitor_sdl_refr_thread(void *param);
static void window_create(monitor_t *m);
static void window_update(monitor_t *m);

/***********************
 *   GLOBAL PROTOTYPES
 ***********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static monitor_t monitor;

#if MONITOR_DUAL
monitor_t monitor2;
#endif

static volatile bool sdl_inited = false;
static volatile bool sdl_quit_qry = false;

int quit_filter(void *userdata, SDL_Event *event);
static void monitor_sdl_clean_up(void);
static void monitor_sdl_init(void);
#ifdef MONITOR_EMSCRIPTEN
void monitor_sdl_refr_core(void); /* called from Emscripten loop */
#else
static void monitor_sdl_refr_core(void);
#endif

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Initialize the monitor
 */
void monitor_init(void)
{
    /*OSX needs to initialize SDL here*/
#if defined(MONITOR_APPLE) || defined(MONITOR_EMSCRIPTEN)
    monitor_sdl_init();
#endif

#ifndef MONITOR_EMSCRIPTEN
    SDL_CreateThread(monitor_sdl_refr_thread, "sdl_refr", NULL);
    while (sdl_inited == false)
        ; /*Wait until 'sdl_refr' initializes the SDL*/
#endif
}

/**
 * Flush a buffer to the marked area
 * @param drv pointer to driver where this function belongs
 * @param area an area where to copy `color_p`
 * @param color_p an array of pixel to copy to the `area` part of the screen
 */
#if 1
void monitor_buffer_flush(uint8_t *buf)
{
    int x = 0;
    int y = 0;
    int i;
    for (y = 0; y < LV_VER_RES_MAX / 8; y++)
    {
        for (x = 0; x < 128; x++)
        {
            uint32_t c;

            for (i = 0; i < 8; i++)
            {
                if (buf[128 * y + x] & (1 << i))
                {
                    // monitor_flush(x, y*8+i, 0);
                    c = 0xff0000ff;
                    // printf("1");
                }
                else
                {
                    c = 0xffffff00;
                    // printf("0");
                    // monitor_flush(x, y*8+i, 3);
                }
                monitor.tft_fb[(y * 8 + i) * LV_HOR_RES_MAX + x] = c;
                // printf("line %d\n", y*8+i);
            }
        }
        // printf("\n");
    }
    monitor.sdl_refr_qry = true;
}

void monitor_flush(Sint32 x, Sint32 y, Uint32 h)
{
    // lv_coord_t hres = disp_drv->rotated == 0 ? disp_drv->hor_res : disp_drv->ver_res;
    // lv_coord_t vres = disp_drv->rotated == 0 ? disp_drv->ver_res : disp_drv->hor_res;

    //    printf("x1:%d,y1:%d,x2:%d,y2:%d\n", area->x1, area->y1, area->x2, area->y2);

    /*Return if the area is out the screen*/
    // if(area->x2 < 0 || area->y2 < 0 || area->x1 > hres - 1 || area->y1 > vres - 1) {

    //     lv_disp_flush_ready(disp_drv);
    //     return;
    // }

#if MONITOR_DOUBLE_BUFFERED
    monitor.tft_fb_act = (uint32_t *)color_p;

    monitor.sdl_refr_qry = true;

    /*IMPORTANT! It must be called to tell the system the flush is ready*/
    lv_disp_flush_ready(disp_drv);
#else

    //     int32_t y;
    // #if LV_COLOR_DEPTH != 24 && LV_COLOR_DEPTH != 32    /*32 is valid but support 24 for backward compatibility too*/
    //     int32_t x;
    //     for(y = area->y1; y <= area->y2 && y < disp_drv->ver_res; y++) {
    //         for(x = area->x1; x <= area->x2; x++) {
    //             monitor.tft_fb[y * disp_drv->hor_res + x] = lv_color_to32(*color_p);
    //             color_p++;
    //         }

    //     }
    // #else
    //     uint32_t w = lv_area_get_width(area);
    //     for(y = area->y1; y <= area->y2 && y < disp_drv->ver_res; y++) {
    //         memcpy(&monitor.tft_fb[y * MONITOR_HOR_RES + area->x1], color_p, w * sizeof(lv_color_t));
    //         color_p += w;
    //     }
    // #endif
    // if (x == 0 && y == 0){
    //     memset(monitor.tft_fb, 0, LV_HOR_RES_MAX * LV_VER_RES_MAX);
    // }
    switch (h)
    {
    case 0:
        monitor.tft_fb[y * LV_HOR_RES_MAX + x] = 0xff000000;
        break;
    case 3:
        monitor.tft_fb[y * LV_HOR_RES_MAX + x] = 0xffffffff;
        break;
    case 4:
        monitor.tft_fb[y * LV_HOR_RES_MAX + x] = 0xffffff00;
        break;
    default:
        monitor.tft_fb[y * LV_HOR_RES_MAX + x] = 0;
        printf("default\n");
        break;
    }

    monitor.sdl_refr_qry = true;
    // printf("%d,%d,%d\n",x,y,h);

    /*IMPORTANT! It must be called to tell the system the flush is ready*/
    // lv_disp_flush_ready(disp_drv);
#endif
}
#endif

#if MONITOR_DUAL

/**
 * Flush a buffer to the marked area
 * @param drv pointer to driver where this function belongs
 * @param area an area where to copy `color_p`
 * @param color_p an array of pixel to copy to the `area` part of the screen
 */
void monitor_flush2(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    lv_coord_t hres = disp_drv->rotated == 0 ? disp_drv->hor_res : disp_drv->ver_res;
    lv_coord_t vres = disp_drv->rotated == 0 ? disp_drv->ver_res : disp_drv->hor_res;

    /*Return if the area is out the screen*/
    if (area->x2 < 0 || area->y2 < 0 || area->x1 > hres - 1 || area->y1 > vres - 1)
    {
        lv_disp_flush_ready(disp_drv);
        return;
    }

#if MONITOR_DOUBLE_BUFFERED
    monitor2.tft_fb_act = (uint32_t *)color_p;

    monitor2.sdl_refr_qry = true;

    /*IMPORTANT! It must be called to tell the system the flush is ready*/
    lv_disp_flush_ready(disp_drv);
#else

    int32_t y;
#if LV_COLOR_DEPTH != 24 && LV_COLOR_DEPTH != 32 /*32 is valid but support 24 for backward compatibility too*/
    int32_t x;
    for (y = area->y1; y <= area->y2 && y < disp_drv->ver_res; y++)
    {
        for (x = area->x1; x <= area->x2; x++)
        {
            monitor2.tft_fb[y * disp_drv->hor_res + x] = lv_color_to32(*color_p);
            color_p++;
        }
    }
#else
    uint32_t w = lv_area_get_width(area);
    for (y = area->y1; y <= area->y2 && y < disp_drv->ver_res; y++)
    {
        memcpy(&monitor2.tft_fb[y * disp_drv->hor_res + area->x1], color_p, w * sizeof(lv_color_t));
        color_p += w;
    }
#endif

    monitor2.sdl_refr_qry = true;

    /*IMPORTANT! It must be called to tell the system the flush is ready*/
    lv_disp_flush_ready(disp_drv);
#endif
}
#endif

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * It will be called from the main SDL thread
 */
bool left_button_down = false;
static int16_t last_x = 0;
static int16_t last_y = 0;
void mouse_handler(SDL_Event *event)
{
    switch (event->type)
    {
    case SDL_MOUSEBUTTONUP:
        if (event->button.button == SDL_BUTTON_LEFT)
            left_button_down = false;
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            left_button_down = true;
            last_x = event->motion.x / MONITOR_ZOOM;
            last_y = event->motion.y / MONITOR_ZOOM;
        }
        break;
    case SDL_MOUSEMOTION:
        last_x = event->motion.x / MONITOR_ZOOM;
        last_y = event->motion.y / MONITOR_ZOOM;

        break;
    }
    printf("x = %d, y = %d\n", last_x, last_y);
}
/**
 * SDL main thread. All SDL related task have to be handled here!
 * It initializes SDL, handles drawing and the mouse.
 */

static int monitor_sdl_refr_thread(void *param)
{
    (void)param;

    /*If not OSX initialize SDL in the Thread*/
#ifndef MONITOR_APPLE
    monitor_sdl_init();
#endif
    /*Run until quit event not arrives*/
    // printf("sdl_ %d\n", sdl_quit_qry);
    while (sdl_quit_qry == false)
    {
        /*Refresh handling*/
        monitor_sdl_refr_core();
    }
    monitor_sdl_clean_up();
    printf("exit\n");
    exit(0);

    return 0;
}

int quit_filter(void *userdata, SDL_Event *event)
{
    (void)userdata;
    // printf("%s in\n", __func__);
    if (event->type == SDL_WINDOWEVENT)
    {
        if (event->window.event == SDL_WINDOWEVENT_CLOSE)
        {
            sdl_quit_qry = true;
        }
    }
    else if (event->type == SDL_QUIT)
    {
        sdl_quit_qry = true;
    }

    return 1;
}

static void monitor_sdl_clean_up(void)
{
    SDL_DestroyTexture(monitor.texture);
    SDL_DestroyRenderer(monitor.renderer);
    SDL_DestroyWindow(monitor.window);

#if MONITOR_DUAL
    SDL_DestroyTexture(monitor2.texture);
    SDL_DestroyRenderer(monitor2.renderer);
    SDL_DestroyWindow(monitor2.window);

#endif

    SDL_Quit();
}

static void monitor_sdl_init(void)
{
    printf("%s\n", __func__);
    /*Initialize the SDL*/
    SDL_Init(SDL_INIT_VIDEO);

    SDL_SetEventFilter(quit_filter, NULL);

    window_create(&monitor);
#if MONITOR_DUAL
    window_create(&monitor2);
    int x, y;
    SDL_GetWindowPosition(monitor2.window, &x, &y);
    SDL_SetWindowPosition(monitor.window, x + MONITOR_HOR_RES / 2 + 10, y);
    SDL_SetWindowPosition(monitor2.window, x - MONITOR_HOR_RES / 2 - 10, y);
#endif

    sdl_inited = true;
}
void mousewheel_handler(SDL_Event *event)
{
    switch (event->type)
    {
    case SDL_MOUSEWHEEL:
        // Scroll down (y = -1) means positive encoder turn,
        // so invert it
#ifdef __EMSCRIPTEN__
        /*Escripten scales it wrong*/
        if (event->wheel.y < 0)
            enc_diff++;
        if (event->wheel.y > 0)
            enc_diff--;
#else
        // enc_diff = -event->wheel.y;
#endif
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (event->button.button == SDL_BUTTON_MIDDLE)
        {
            // state = LV_INDEV_STATE_PR;
        }
        break;
    case SDL_MOUSEBUTTONUP:
        if (event->button.button == SDL_BUTTON_MIDDLE)
        {
            // state = LV_INDEV_STATE_REL;
        }
        break;
    default:
        break;
    }
}

void keyboard_handler(SDL_Event *event)
{
    /* We only care about SDL_KEYDOWN and SDL_KEYUP events */
    switch (event->type)
    {
    case SDL_KEYDOWN: /*Button press*/
        // printf("key down %c\n", event->key.keysym.sym);
        digitalRead2(event->key.keysym.sym);
        break;
    case SDL_KEYUP: /*Button release*/

        break;
    default:
        break;
    }
}

#ifdef MONITOR_EMSCRIPTEN
void monitor_sdl_refr_core(void)
#else
static void monitor_sdl_refr_core(void)
#endif
{

    if (monitor.sdl_refr_qry != false)
    {
        // monitor.sdl_refr_qry = false;
        window_update(&monitor);
    }

#if MONITOR_DUAL
    if (monitor2.sdl_refr_qry != false)
    {
        monitor2.sdl_refr_qry = false;
        window_update(&monitor2);
    }
#endif
#if 1
#if !defined(MONITOR_APPLE) && !defined(MONITOR_EMSCRIPTEN)
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
#if USE_MOUSE != 0
        mouse_handler(&event);
#endif
        mousewheel_handler(&event);

#if USE_KEYBOARD
        keyboard_handler(&event);
#endif
        if ((&event)->type == SDL_WINDOWEVENT)
        {
            switch ((&event)->window.event)
            {
#if SDL_VERSION_ATLEAST(2, 0, 5)
            case SDL_WINDOWEVENT_TAKE_FOCUS:
#endif
            case SDL_WINDOWEVENT_EXPOSED:
                window_update(&monitor);
#if MONITOR_DUAL
                window_update(&monitor2);
#endif
                break;
            default:
                break;
            }
        }
    }
#endif /*MONITOR_APPLE*/
#endif
    /*Sleep some time*/
    SDL_Delay(SDL_REFR_PERIOD);
}

static void window_create(monitor_t *m)
{
    printf("%s in\n", __func__);
    m->window = SDL_CreateWindow("TFT Simulator",
                                 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                 MONITOR_HOR_RES * MONITOR_ZOOM, MONITOR_VER_RES * MONITOR_ZOOM, 0); /*last param. SDL_WINDOW_BORDERLESS to hide borders*/

#if MONITOR_VIRTUAL_MACHINE || defined(MONITOR_EMSCRIPTEN)
    m->renderer = SDL_CreateRenderer(m->window, -1, SDL_RENDERER_SOFTWARE);
#else
    m->renderer = SDL_CreateRenderer(m->window, -1, 0);
#endif
    m->texture = SDL_CreateTexture(m->renderer,
                                   SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, MONITOR_HOR_RES, MONITOR_VER_RES);
    SDL_SetTextureBlendMode(m->texture, SDL_BLENDMODE_BLEND);

    /*Initialize the frame buffer to gray (77 is an empirical value) */
#if MONITOR_DOUBLE_BUFFERED
    SDL_UpdateTexture(m->texture, NULL, m->tft_fb_act, MONITOR_HOR_RES * sizeof(uint32_t));
#else
    // memset(m->tft_fb, 0x90, MONITOR_HOR_RES * MONITOR_VER_RES * sizeof(uint32_t));

#endif

    m->sdl_refr_qry = true;
}
// 480 320
// SDL_PIXELFORMAT_ARGB8888
static void window_update(monitor_t *m)
{
#if MONITOR_DOUBLE_BUFFERED == 0
    SDL_UpdateTexture(m->texture, NULL, m->tft_fb, LV_HOR_RES_MAX * sizeof(uint32_t));
#else
    if (m->tft_fb_act == NULL)
        return;
    SDL_UpdateTexture(m->texture, NULL, m->tft_fb_act, MONITOR_HOR_RES * sizeof(uint32_t));
#endif
    SDL_RenderClear(m->renderer);
    /*Test: Draw a background to test transparent screens (LV_COLOR_SCREEN_TRANSP)*/
    //        SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 0xff);
    //        SDL_Rect r;
    //        r.x = 0; r.y = 0; r.w = MONITOR_HOR_RES; r.w = MONITOR_VER_RES;
    //        SDL_RenderDrawRect(renderer, &r);

    /*Update the renderer with the texture containing the rendered image*/
    SDL_RenderCopy(m->renderer, m->texture, NULL, NULL);
    SDL_RenderPresent(m->renderer);
}
