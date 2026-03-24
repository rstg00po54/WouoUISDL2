
#include "u8g2.h"
#include <stdio.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "Arduino.h"
#include <stdbool.h>
#include <monitor.h>

#include "../WouoUI-Demo/ui-128_64/WouoUI-128_64.h"
extern volatile bool sdl_quit_qry;

void postLoop();


int main(int argc, char *argv[])
{
    printf("pro in\n");
    setup();
    while (1){
        loop();
        // postLoop();
        SDL_Delay(5);

    }
    printf("pro out\n");
    return 1;
}

