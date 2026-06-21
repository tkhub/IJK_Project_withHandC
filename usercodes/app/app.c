/**
 * @file app.c
 * @brief 簡単な説明
 * @author tkp
 * @date 30
 * @Version 0.00
 */

/*========VVVV Include Standard Header START VVVV============================*/
/*========AAAA Include Standard Header END AAAA==============================*/

/*========VVVV Include Local Header START VVVV===============================*/
#include "app.h"
#include "app_if.h"
#include <xprintf.h>
#include "linmkrssr/linmkrssr.h"
#include "motor/motor.h"
#include "sequencer.h"
#include "tracectrl.h"

/*========AAAA Include Local Header END AAAA=================================*/

/*========VVVV Typedef Definition START VVVV=================================*/
/*========AAAA Typedef Definition END AAAA===================================*/

/*========VVVV MACRO Definition START VVVV===================================*/
/**
 * @brief マクロ MAX_NANK の説明
 */
/* #define MAX_NANKA 256 */
#define ZANTEI_WAIT_INIT 	0
#define ZANTEI_WAIT_SW2 	1
#define ZANTEI_WAIT_STARTGT 2
#define ZANTEI_WAIT_PASSCRS 3
#define ZANTEI_WAIT_GOALGT 	4
#define ZANTEI_WAIT_STOP 	5

/*========AAAA MACRO Definition END AAAA=====================================*/

/*========VVVV GLOBAL Variable Definition START VVVV=========================*/
/* int global_var; */ /* ヘッダファイルで説明済み */
/*========AAAA GLOBAL Variable Definition END AAAA===========================*/

/*========VVVV Private Variable Definition START VVVV========================*/
/**
 * @brief 内部グローバル変数 internal_var の説明
 */
/* static int internal_var; */
/*========AAAA Private Variable Definition END AAAA==========================*/

/*========VVVV Private Function Prototype Declaration START VVVV=============*/
/**
 * @brief 内部テスト関数
 * @param [in/out] buffer バッファ
 * @param [in] size バッファのサイズ
 * @return 戻り値の説明
 * @details 詳細な説明
 */
/* static int internalTestFunc(int* buffer, int size); */
volatile static sensors_t   ijk_sensors;
volatile static drives_t    ijk_drives;
volatile static uiout_t     ijk_uiout;
volatile static uiin_t      ijk_uiin;
volatile static uint16_t 	zanteiCnt;
volatile static uint8_t zanteiSq = ZANTEI_WAIT_INIT;
/*========AAAA Private Function Prototype Declaration END AAAA===============*/

/*========VVVV GLOBAL Function Definition START VVVV=========================*/
/*
 * int testFunc(int* buffer, int size) {
 *
 * }
*/
/** APPの初期化関数 */
void appInit(void) {
    tracectrlInit();
    sequencerInit();
}

/** APPの1msインターバルハンドラ */
void appIntervalHandler_1ms(void) {
    ijk_sensors.linesensor.position = linesensorsReadPosition();
    ijk_sensors.linesensor.binaries = linesensorsReadBin();
    ijk_sensors.markerFlags = markersensorsRead();
    sequencerInterval_1ms(ijk_sensors, 10);
    traceCtrInterval_1ms(ijk_sensors, ijk_drives);
}

/** APPの10msインターバルハンドラ */
void appIntervalHandler_10ms(void) {
}

/** APPのメインループ */
void appMainLoop(void){
    markerevent_t mkev;
    buzzerSchedule_t bz;
    float mL, mR, pos;
    pos = linesensorsReadPosition();
    motorsReadPower(&mL, &mR);
    ijk_uiin.uiswevent = uiswRead();
    mkev = markerReadEvent();
    switch (zanteiSq) {
    case ZANTEI_WAIT_INIT:
        ijk_drives.powerLR = 0.0;
        zanteiSq = ZANTEI_WAIT_SW2;
        xprintf("ZANTEI INIT\r\n");
        break;
    case ZANTEI_WAIT_SW2:
        if (ijk_uiin.uiswevent == UISW2_PUSH_EVENT) {
            ijk_drives.powerLR = 0.3;
            bz.oncount10ms = 50;
            bz.offcount10ms = 1;
            buzzerSetSchedule(bz);
            zanteiSq = ZANTEI_WAIT_STARTGT;
            xprintf("ZANTEI SW2\r\n");
        }
        break;

    case ZANTEI_WAIT_STARTGT:
        if (mkev == MARKER_EVENT_GOAL) {
            ijk_drives.powerLR = 0.50;
            bz.oncount10ms = 50;
            bz.offcount10ms = 25;
            buzzerSetSchedule(bz);
            buzzerSetSchedule(bz);
            zanteiSq = ZANTEI_WAIT_GOALGT;
            xprintf("ZANTEI START\r\n");
        }
        break;

    case ZANTEI_WAIT_GOALGT:
        xprintf("%f,\t%f,\t%f", pos, mL, mR);
        if (mkev == MARKER_EVENT_CORNER) {
            bz.oncount10ms = 50;
            bz.offcount10ms = 25;
            buzzerSetSchedule(bz);
            zanteiSq = ZANTEI_WAIT_PASSCRS;
            zanteiCnt = 5;
            xprintf("ZANTEI CORNER");
        }
        else if ((mkev == MARKER_EVENT_CROSS) ||
                (ijk_sensors.linesensor.binaries == 0x09) ||
                (ijk_sensors.linesensor.binaries == 0x0F) ||
                (ijk_sensors.linesensor.binaries == 0x0B) ||
                (ijk_sensors.linesensor.binaries == 0x0D) ||
                (ijk_sensors.linesensor.binaries == 0x0A) ||
                (ijk_sensors.linesensor.binaries == 0x05) ){
            bz.oncount10ms = 25;
            bz.offcount10ms = 25;
            buzzerSetSchedule(bz);
            buzzerSetSchedule(bz);
            zanteiSq = ZANTEI_WAIT_PASSCRS;
            zanteiCnt = 50;
            xprintf("ZANTEI CROSS");
        }

        else if (mkev == MARKER_EVENT_GOAL) {
            ijk_drives.powerLR = 0.2;
            bz.oncount10ms = 20;
            bz.offcount10ms = 10;
            buzzerSetSchedule(bz);
            buzzerSetSchedule(bz);
            buzzerSetSchedule(bz);
            zanteiSq = ZANTEI_WAIT_STOP;
            zanteiCnt = 100;
            xprintf("ZANTEI STOP");
        }
        xprintf("\n\r");
        break;

    case ZANTEI_WAIT_PASSCRS:
        if (zanteiCnt == 0) {
            zanteiSq = ZANTEI_WAIT_GOALGT;
        }
        else {
            zanteiCnt--;
        }
        break;

    case ZANTEI_WAIT_STOP:

        if (zanteiCnt == 0) {
            ijk_drives.powerLR = 0.0;
            bz.oncount10ms = 10;
            bz.offcount10ms = 10;
            buzzerSetSchedule(bz);
            buzzerSetSchedule(bz);
            buzzerSetSchedule(bz);
            buzzerSetSchedule(bz);
            buzzerSetSchedule(bz);
            zanteiSq = ZANTEI_WAIT_INIT;
            xprintf("ZANTEI END\n\r");
        }
        else if (mkev == MARKER_EVENT_CORNER) {
            zanteiSq = ZANTEI_WAIT_GOALGT;
        }
        else {
            zanteiCnt--;
        }
    break;
    default :
        zanteiSq = ZANTEI_WAIT_INIT;
        break;
    }
    
}




//        if (rnsq == RUN_SQ_WAIT_STARTMKR) {
//            ijk_drives.powerLR = 0.3;
//            if (rnsq_last != rnsq) {
//                bz.oncount10ms = 50;
//                bz.offcount10ms = 50;
//                buzzerSetSchedule(bz);
//            }
//        }
//        else if (rnsq == RUN_SQ_WAIT_GOALMKR) {
//            ijk_drives.powerLR = 0.4;
//            if (rnsq_last != rnsq) {
//                bz.oncount10ms = 50;
//                bz.offcount10ms = 25;
//                buzzerSetSchedule(bz);
//                bz.oncount10ms = 50;
//                bz.offcount10ms = 25;
//                buzzerSetSchedule(bz);
//            }
//        }
//        else if (rnsq == RUN_SQ_WAIT_STOP) {
//            ijk_drives.powerLR = 0.1;
//            if (rnsq_last != rnsq) {
//                bz.oncount10ms = 50;
//                bz.offcount10ms = 25;
//                buzzerSetSchedule(bz);
//                bz.oncount10ms = 50;
//                bz.offcount10ms = 25;
//                buzzerSetSchedule(bz);
//                bz.oncount10ms = 50;
//                bz.offcount10ms = 25;
//                buzzerSetSchedule(bz);
//            }
//        }
//        else {
//            ijk_drives.powerLR = 0.0;
//            if (rnsq_last != rnsq) {
//                bz.oncount10ms = 100;
//                bz.offcount10ms = 100;
//                buzzerSetSchedule(bz);
//            }
//        }
//        if (mkev == MARKER_EVENT_CORNER) {
//            bz.oncount10ms = 20;
//            bz.offcount10ms = 20;
//            buzzerSetSchedule(bz);
//        }
//        else if (mkev == MARKER_EVENT_CROSS) {
//            bz.oncount10ms = 20;
//            bz.offcount10ms = 20;
//            buzzerSetSchedule(bz);
//            bz.oncount10ms = 20;
//            bz.offcount10ms = 20;
//            buzzerSetSchedule(bz);
//        }
//        else if (mkev == MARKER_EVENT_OVERRUN) {
//            bz.oncount10ms = 200;
//            bz.offcount10ms = 50;
//            buzzerSetSchedule(bz);
//            bz.oncount10ms = 200;
//            bz.offcount10ms = 50;
//            buzzerSetSchedule(bz);
//            bz.oncount10ms = 200;
//            bz.offcount10ms = 50;
//            buzzerSetSchedule(bz);
//        }
//        else {
//            /* NOP */
//        }
//        rnsq_last = rnsq;
//        linesensorsDebug(rawVal, nrmVal);
//        xprintf("line = %f, %d, [%d, %d, %d, %d], [%f, %f, %f, %f], mkr = %d, run=%d, %d, %d\n\r",
//        		ijk_sensors.linesensor.position, ijk_sensors.linesensor.binaries,
//				rawVal[3], rawVal[2], rawVal[1], rawVal[0], nrmVal[3], nrmVal[2], nrmVal[1], nrmVal[0],
//				ijk_sensors.markerFlags,
//				rnsq, mkev, markerReadSequence());


/*========AAAA GLOBAL Function Definition END AAAA===========================*/

/*========VVVV Private Function Definition START VVVV========================*/
/* static int internalTestFunc(int* buffer, int size) {
 *
 * }
*/
/*========AAAA Private Function Definition END AAAA==========================*/