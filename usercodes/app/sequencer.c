/*
 * sequencer.c
 *
 *  Created on: Oct 30, 2025
 *      Author: takap
 */


/*========VVVV Include Standard Header START VVVV============================*/
#include "orgtypedef.h"
/*========AAAA Include Standard Header END AAAA==============================*/

/*========VVVV Include Local Header START VVVV===============================*/
#include "sequencer.h"
#include "app_if.h"


/*========AAAA Include Local Header END AAAA=================================*/

/*========VVVV Typedef Definition START VVVV=================================*/



/*========AAAA Typedef Definition END AAAA===================================*/

/*========VVVV MACRO Definition START VVVV===================================*/
#define MARKER_GOAL(BYTE)   (BYTE == 0x01)
#define MARKER_CORNER(BYTE) (BYTE == 0x02)
#define MARKER_CROSS(BYTE)  (MARKER_GOAL(BYTE) && MARKER_CORNER(BYTE))
#define LINE_CROSS(BYTE)    ( (BYTE == 0x09) || (BYTE == 0x0F) ) // TODO:とりあえず両端が反応 or 全反応をクロスライン時の値としておく

/*========AAAA MACRO Definition END AAAA=====================================*/

/*========VVVV GLOBAL Variable Definition START VVVV=========================*/
/*========AAAA GLOBAL Variable Definition END AAAA===========================*/

/*========VVVV Private Variable Definition START VVVV========================*/
/*! @var   global_var1
    @brief グローバル変数の説明
*/
volatile static markerevent_t  markerevent;
volatile static markersq_t markersequence;
volatile static uint8_t markerLastBits;
volatile static uint16_t markerMaskCnt;
volatile static runsq_t runsequence;
volatile static uint16_t runsqStopWaitCnt;



/*========AAAA Private Variable Definition END AAAA==========================*/

/*========VVVV Private Function Prototype Declaration START VVVV=============*/
/**
 * @fn
 * ここに関数の説明を書く
 * @brief 要約説明
 * @param (引数名) 引数の説明
 * @param (引数名) 引数の説明
 * @return 戻り値の説明
 * @sa 参照すべき関数を書けばリンクが貼れる
 * @detail 詳細な説明
 */
void markerSequencerInit(void);
void markerSequencer_1ms(uint8_t markerFlags, uint8_t linebin, uint16_t maskTime1ms);
markerevent_t markerReadEvent(void);
void runSequencer(markerevent_t mkevnt, uiin_t uii);
/*========AAAA Private Function Prototype Declaration END AAAA===============*/

/*========VVVV GLOBAL Function Definition START VVVV=========================*/

void sequencerInit(void) {

    markerSequencerInit();
}

void sequencerInterval_1ms(sensors_t allSensors, uint16_t maskTime1ms) {
    markerSequencer_1ms(allSensors.markerFlags, allSensors.linesensor.binaries, maskTime1ms);
}

void sequencerInterval_10ms(void) {
    if (runsqStopWaitCnt != 0) {
        runsqStopWaitCnt--;
    }
}

void sequencerLoop(uiin_t uii, runsq_t* runsq, markerevent_t* mkrev) {
    runSequencer(markerReadEvent(), uii);
    *runsq = runsequence;
    *mkrev = markerevent;
}

/*========AAAA GLOBAL Function Definition END AAAA===========================*/

/*========VVVV Private Function Definition START VVVV========================*/


//typedef enum {RUN_SQ_INIT, RUN_SQ_WAIT_SW2, RUN_SQ_WAIT_STARTMKR, RUN_SQ_WAIT_GOALMKR, RUN_SQ_WAIT_STOP} runsq_t;

void runSequencer(markerevent_t mkevnt, uiin_t uii) {
    // ひつようなもの。IN：スイッチ、マーカー。OUT：速度、ブザーとか
    switch (runsequence) {
    default:
        runsequence = RUN_SQ_WAIT_SW2;
        break;
    case RUN_SQ_WAIT_SW2:
        if (uii.uiswevent == UISW2_PUSH_EVENT) {
            runsequence = RUN_SQ_WAIT_STARTMKR;
        }
        break;
    case RUN_SQ_WAIT_STARTMKR:
        // スタートマーカー通過待ち
        if (mkevnt == MARKER_EVENT_GOAL) {
            runsequence = RUN_SQ_WAIT_GOALMKR;
        }
    case RUN_SQ_WAIT_GOALMKR:
        //　通常の走行
        if (mkevnt == MARKER_EVENT_GOAL) {
            runsequence = RUN_SQ_WAIT_STOP;
            runsqStopWaitCnt = 100;
        }
    case RUN_SQ_WAIT_STOP:
        if (runsqStopWaitCnt == 0) {
            runsequence = RUN_SQ_WAIT_SW2;
        }
    }
}

void markerSequencerInit(void) {
    markerevent = MARKER_EVENT_NON;
    markersequence = MARKER_SQ_INIT;
}

void markerSequencer_1ms(uint8_t markerFlags, uint8_t linebin, uint16_t maskTime1ms) {
//    markerevent_t eventtmp = MARKER_EVENT_NON;
    switch (markersequence) {
    default : // MARKER_SQ_INIT
        markersequence = MARKER_SQ_WAIT_MARKER;
        break;
    case MARKER_SQ_WAIT_MARKER:
        if      (LINE_CROSS(linebin)) {
            //　ラインセンサがクロスラインを最初に拾った場合、マスク状態に入る
//            eventtmp = MARKER_EVENT_CROSS;
            markerevent = MARKER_EVENT_CROSS;
            markersequence = MARKER_SQ_WAIT_MASK;
            markerMaskCnt = maskTime1ms;
        }
        else if (markerFlags == 0x03) {
            // おそらくここには入らないが。。。
//            eventtmp = MARKER_EVENT_CROSS;
        	markerevent = MARKER_EVENT_CROSS;
            markersequence = MARKER_SQ_WAIT_PASSING;
        }

        else if (markerFlags == 0x02) {
//            eventtmp = MARKER_EVENT_CORNER;
            markerevent = MARKER_EVENT_CORNER;
            markersequence = MARKER_SQ_WAIT_PASSING;
        }
        else if (markerFlags == 0x01)  {
//            eventtmp = MARKER_EVENT_GOAL;
        	markerevent = MARKER_EVENT_GOAL;
            markersequence = MARKER_SQ_WAIT_PASSING;
        }
        else {
        	/* NOP */
		}
        break;
    case MARKER_SQ_WAIT_PASSING:
        if ( !markerFlags && !LINE_CROSS(linebin) ) {
            // センサの位置関係上、クロスラインとマーカーを同時検出し得ない
            markersequence = MARKER_SQ_WAIT_MARKER;
        }
        break;
    case MARKER_SQ_WAIT_MASK:
        if (markerMaskCnt == 0) {
            markersequence = MARKER_SQ_WAIT_PASSING;
        }
        else {
            markerMaskCnt--;
        }
        break;
    case MARKER_SQ_ERROR:
        markersequence =MARKER_SQ_INIT;
    }
    markerLastBits = markerFlags;
}

markerevent_t markerReadEvent(void) {
	markerevent_t tmp = markerevent;
	markerevent = MARKER_EVENT_NON;
    return tmp;
}

markersq_t markerReadSequence(void) {
	return markersequence;
}

/*========AAAA Private Function Definition END AAAA==========================*/

