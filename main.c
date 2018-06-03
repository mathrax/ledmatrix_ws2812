#include <plib.h>
#include <stdlib.h>

#include "./user.h"
#include "./ledmatrix_ws2812.h"

#include "./animation_data/pattern.h"       //HEART_REV, BREAK_HEART
#include "./animation_data/energy.h"        //ENERGY
#include "./animation_data/broken.h"        //BROKEN
#include "./animation_data/normal_blink.h"  //NOAMRL_BLINK
#include "./animation_data/startup.h"       //STARTUP
#include "./animation_data/wave.h"          //WAVE
#include "./animation_data/hanabi.h"        //HANABI

#include "./animation_data/shinobi_ompu_guitar_20180602.h"

unsigned char aCnt;
unsigned char frameCount;
unsigned char firstReset;

void __ISR(_UART_1_VECTOR, IPL4) U1RXHandler(void) {
    unsigned char RcvData;
    IFS1bits.U1RXIF = 0;
    RcvData = getcUART1();
    if (RcvData == 255) {
        dataPos = 0;
    } else {
        if (
                RcvData == 'X' || RcvData == 'G'
                || RcvData == 'O' || RcvData == 'P'

                || RcvData == 'U' || RcvData == 'D'
                || RcvData == 'L' || RcvData == 'R'

                || RcvData == 'g' //STK-L LEFT
                || RcvData == 'h' //STK-L RIGHT
                || RcvData == 'i' //STK-L UP
                || RcvData == 'j' //STK-L DOWN

                || RcvData == 'k' //STK-R LEFT
                || RcvData == 'l' //STK-R RIGHT
                || RcvData == 'm' //STK-R UP
                || RcvData == 'n' //STK-R DOWN


                || RcvData == 'a' //L1
                || RcvData == 'b' //R1
                || RcvData == 'c' //L2
                || RcvData == 'd' //R2
                ) {

            if (lastData != RcvData) {
                myData[dataPos] = RcvData;

                if (RcvData == 'k' || RcvData == 'l' || RcvData == 'h' || RcvData == 'i' || RcvData == 'g'
                        || RcvData == 'U' || RcvData == 'D' || RcvData == 'L' || RcvData == 'R'
                        || RcvData == 'X'

                        || RcvData == 'm' //STK-R UP
                        || RcvData == 'n' //STK-R DOWN
                        || RcvData == 'g' //STK-L LEFT
                        || RcvData == 'j' //STK-L DOWN

                        ) {
                    frameCount = 0;
                    aCnt = 0;
                } else {
                    deletePattern();
                }
            }
            dataPos++;

            if (dataPos >= 2)dataPos = 0;
        }
    }
    lastData = myData[0];
}

int main(void) {
    unsigned int i;


    SYSTEMConfigPerformance(SYSCLK);
    INTEnableSystemMultiVectoredInt();
    INTEnableInterrupts();

    ANSELA = 0x0000; // all digital pins
    ANSELB = 0x0000;

    TRISB = 0;
    TRISA = 0;

    InitUart1();

    resetAnimation();

    myData[0] = -1;
    while (1) {
        frameCount++;
        switch (myData[0]) {

            default:
                setPattern(break_heart, 1);
                break;


            case 0:
                deletePattern();
                break;

                //UP
            case 'U':
                //

                break;

                //DOWN
                //HANABI
            case 'D':

                if (frameCount % 3 == 0) {
                    frameCount = 0;
                    aCnt++;
                    if (aCnt >= sizeof (frame_hanabi) / sizeof (unsigned char)) {
                        aCnt = sizeof (frame_hanabi) / sizeof (unsigned char) - 1;
                        myData[0] = 0;
                        deletePattern();
                    }
                }
                setPattern(hanabi[frame_hanabi[aCnt]], 2);
                break;


                //LEFT
            case 'L':
                //NORMAL BLINK
                if (frameCount % 16 == 0) {
                    frameCount = 0;
                    aCnt++;
                    if (aCnt >= sizeof (frame_normal_blink) / sizeof (unsigned char)) {
                        aCnt = 0;
                    }
                }
                setPattern(normal_blink[frame_normal_blink[aCnt]], 2);
                break;

                //RIGHT 
            case 'R':
                //BROKEN BLINK
                if (frameCount % 3 == 0) {
                    frameCount = 0;

                    aCnt++;
                    if (aCnt >= sizeof (frame_broken) / sizeof (unsigned char)) {
                        aCnt = 0;
                    }
                }
                setPattern(broken[frame_broken[aCnt]], 2);
                break;

                //BATSU
                //DELETE
            case 'X':
                myData[0] = 0;
                deletePattern();
                break;

                //SANKAKU
            case 'G':
                //BATSU
                setPattern(batsu, 1);
                break;

                //MARU
            case 'O':
                //HATENA?
                setPattern(hatena, 1);
                break;

                //SIKAKU
            case 'P':
                //BREAK HEART
                setPattern(break_heart, 1);
                break;

                //STK-L LEFT
            case 'g':

                //WAVE
                if (frameCount % 3 == 0) {
                    aCnt++;
                    if (aCnt >= sizeof (frame_wave) / sizeof (unsigned char)) {
                        aCnt = 0;
                    }
                }
                setPattern(wave[frame_wave[aCnt]], 1);


                break;
                //STK-L RIGHT
                //ENERGY
            case 'h':
                if (frameCount % 4 == 0) {
                    frameCount = 0;
                    aCnt++;
                    if (aCnt >= sizeof (frame_energy) / sizeof (unsigned char)) {
                        aCnt = sizeof (frame_energy) / sizeof (unsigned char) - 1;
                    }
                }
                setPattern(energy[frame_energy[aCnt]], 2);
                break;

                //STK-L UP
                //START UP
            case 'i':
                if (frameCount % 4 == 0) {
                    frameCount = 0;
                    aCnt++;
                    if (aCnt >= sizeof (frame_startup) / sizeof (unsigned char)) {
                        aCnt = sizeof (frame_startup) / sizeof (unsigned char) - 1;

                        //After playing, DELETE
                        myData[0] = 0;
                        deletePattern();
                    }
                }
                setPattern(startup[frame_startup[aCnt]], 2);
                break;

                //STK-L DOWN
            case 'j':
                //SHINOBI
                setPattern(shinobi, 2);
                break;

                //STK-R LEFT
            case 'k':
                //ONPU1
                setPattern(onpu1, 2);
                break;

                //STK-R RIGHT
            case 'l':
                //ONPU0
                setPattern(onpu0, 2);
                break;

                //STK-R UP
            case 'm':
                //ONPU2
                setPattern(onpu2, 2);
                break;

                //STK-R DOWN
            case 'n':
                //GUITAR
                setPattern(guitar, 2);
                break;
        }

        show();


    }
}

