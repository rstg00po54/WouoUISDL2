#include <unordered_map>
#include <SDL_events.h>
#include "WouoFix.h"


//每次循环加上10毫秒延迟
extern "C" void postLoop(){
    delay(10);
}


bool takeOver = false;
long longPress = -1;


std::unordered_map<int32_t, bool> keyStatus;


void knob_inter();
void btn_scan();
#ifdef __cplusplus
extern "C" {
#endif

extern int digitalRead2(int pin);

#ifdef __cplusplus
}
#endif

static int keyCode;

int digitalRead2(int pin) {
    keyCode = pin;
    return 0;
}

int digitalRead(int pin) {
    if (takeOver) {
        if (longPress == 0) {
            keyStatus[pin] = !keyStatus[pin];
            longPress--;
        }
        if (longPress > 0) {
            longPress--;
        }
        return keyStatus[pin];
    }



    if(keyCode == 0)
        return keyStatus[pin];
    switch (keyCode) {
        case 'q': {
            // 模拟编码器
            takeOver = true;
            keyStatus[PB12] = 1;
            keyStatus[PB13] = 0;
            knob_inter();
            keyStatus[PB12] = 0;
            keyStatus[PB13] = 1;
            knob_inter();
            keyStatus[PB12] = 1;
            keyStatus[PB13] = 0;
            knob_inter();
            keyStatus[PB12] = 0;
            keyStatus[PB13] = 1;
            knob_inter();
            takeOver = false;

        }
            break;
        case 'w': {
            // 模拟编码器
            takeOver = true;
            keyStatus[PB12] = 1;
            keyStatus[PB13] = 1;
            knob_inter();
            keyStatus[PB12] = 0;
            keyStatus[PB13] = 0;
            knob_inter();
            keyStatus[PB12] = 1;
            keyStatus[PB13] = 1;
            knob_inter();
            keyStatus[PB12] = 0;
            keyStatus[PB13] = 0;
            knob_inter();
            takeOver = false;

        }
            break;
        case 'e': {
            // 模拟短按
            takeOver = true;
            keyStatus[PB14] = 1;
            btn_scan();
            keyStatus[PB14] = 1;
            btn_scan();
            keyStatus[PB14] = 0;
            longPress = 3;
            btn_scan();
            takeOver = false;
        }
            break;
        case 'r': {
            // 模拟长按
            takeOver = true;
            keyStatus[PB14] = 1;
            btn_scan();
            keyStatus[PB14] = 1;
            btn_scan();
            keyStatus[PB14] = 0;
            longPress = 400;
            btn_scan();
            takeOver = false;
        }
            break;

        default: {
        }
    }
//                 break;
//             case SDLK_UP:
//                 switch (event.key.keysym.sym) {
//                     case 'e':
// //                        keyStatus[PB14] = 0;
//                         break;
//                     case 'r':
//                         break;
//                 }
//         }
//     }
    keyCode = 0;

    return keyStatus[pin];
}

