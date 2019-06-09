
#include <plib.h>

#include <stdlib.h>

#include "./user.h"
#include "./ledmatrix_ws2812.h"
#include "./vsc3_keyname.h"

#include "./animation_data/pattern.h"       //HEART_REV, BREAK_HEART
#include "./animation_data/broken.h"        //BROKEN
#include "./animation_data/startup.h"       //STARTUP
#include "./animation_data/hanabi.h"        //HANABI
#include "./animation_data/sleep_heart.h"   //SLEEP HEART
#include "./animation_data/normal_blink.h"  //NORMAL_BLINK
#include "./animation_data/star.h"          //STAR

#include "./animation_data/wave.h"          //WAVE
#include "./animation_data/fire.h"          //FIRE
#include "./animation_data/energy.h"        //ENERGY
#include "./animation_data/dokuro.h"        //DOKURO
#include "./animation_data/kiba.h"        //KIBA


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
                RcvData == BATSU
                || RcvData == SANKAKU
                || RcvData == MARU
                || RcvData == SHIKAKU

                || RcvData == UP
                || RcvData == DOWN
                || RcvData == LEFT
                || RcvData == RIGHT

                || RcvData == STK_L_LEFT
                || RcvData == STK_L_RIGHT
                || RcvData == STK_L_UP
                || RcvData == STK_L_DOWN

                || RcvData == STK_R_LEFT
                || RcvData == STK_R_RIGHT
                || RcvData == STK_R_UP
                || RcvData == STK_R_DOWN


                || RcvData == 'a' //L1
                || RcvData == 'b' //R1
                || RcvData == 'c' //L2
                || RcvData == 'd' //R2

                ) {

            if (lastData != RcvData) {
                myData[dataPos] = RcvData;

                if (       
                        RcvData == UP
                        || RcvData == DOWN
                        || RcvData == LEFT
                        || RcvData == RIGHT
                        || RcvData == BATSU

                        || RcvData == STK_L_LEFT
                        || RcvData == STK_L_RIGHT
                        || RcvData == STK_L_UP
                        || RcvData == STK_L_DOWN

                        || RcvData == STK_R_LEFT
                        || RcvData == STK_R_RIGHT
                        || RcvData == STK_R_UP
                        || RcvData == STK_R_DOWN

                        ) {
                    frameCount = 0;
                    aCnt = 0;

                } else {
                    deletePattern();
                }
            }else if(
                    RcvData == STK_L_DOWN){
                    frameCount = 0;
                    aCnt = 0;
               
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


            case UP:
                //START UP
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



            case DOWN:
                //HANABI
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



            case LEFT:
                //NORMAL BLINK
                if (frameCount % 16 == 0) {
                    aCnt++;
                    if (aCnt >= sizeof (frame_normal_blink) / sizeof (unsigned char)) {
                        aCnt = 0;
                    }
                }
                setPattern(normal_blink[frame_normal_blink[aCnt]], 0);
                break;

            case RIGHT:
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



            case BATSU:
                //DELETE
                myData[0] = 0;
                deletePattern();
                break;


            case SANKAKU:
                //BATSU
                setPattern(batsu, 1);
                break;


            case MARU:
                //HATENA?
                setPattern(hatena, 1);
                break;


            case SHIKAKU:
                //BREAK HEART
                setPattern(break_heart, 2);
                break;


            case STK_L_LEFT:
                //STAR
                if (frameCount % 3 == 0) {
                    frameCount = 0;

                    aCnt++;
                    if (aCnt >= sizeof (frame_star) / sizeof (unsigned char)) {
                        aCnt = 0;
                    }
                }
                setPattern(star[frame_star[aCnt]], 3);
                break;


            case STK_L_RIGHT:
                //DOKURO
                setPattern(dokuro, 3);
                break;


            case STK_L_UP:
                //WAVE
                if (frameCount % 3 == 0) {
                    frameCount = 0;

                    aCnt++;
                    if (aCnt >= sizeof (frame_wave) / sizeof (unsigned char)) {
                        aCnt = 0;
                    }
                }
                setPattern(wave[frame_wave[aCnt]], 2);

                break;


            case STK_L_DOWN:
                //KIBA
                if (frameCount % 4 == 0) {
                    frameCount = 0;

                    aCnt++;
                    if (aCnt >= sizeof (frame_kiba) / sizeof (unsigned char)) {
                        aCnt = sizeof (frame_kiba) / sizeof (unsigned char) - 1;
                    }
                }
                setPattern(kiba[frame_kiba[aCnt]], 3);
                break;


            case STK_R_LEFT:
                //MIDDLE HEART
                setPattern(sleep_heart[frameSleepHeart_1[1]], 2);
                break;

            case STK_R_RIGHT:
                //SMALL HEART
                setPattern(sleep_heart[frameSleepHeart_1[2]], 2);
                break;

            case STK_R_UP:
                //DELETE
                myData[0] = 0;
                deletePattern();
                break;

            case STK_R_DOWN:
                //ENERGY
                if (frameCount % 3 == 0) {
                    frameCount = 0;
                    aCnt++;
                    if (aCnt >= sizeof (frame_energy) / sizeof (unsigned char)) {
                        aCnt = sizeof (frame_energy) / sizeof (unsigned char) - 1;
                    }
                }
                setPattern(energy[frame_energy[aCnt]], 2);
                break;
        }

        show();


    }
}

