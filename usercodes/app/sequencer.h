/*
 * sequencer.h
 *
 *  Created on: Oct 30, 2025
 *      Author: takap
 */

#ifndef SRC_USER_APP_SEQUENCER_H_
#define SRC_USER_APP_SEQUENCER_H_

/*========VVVV Include Standard Header START VVVV============================*/
#include "orgtypedef.h"
/*========AAAA Include Standard Header END AAAA==============================*/

/*========VVVV Include Local Header START VVVV===============================*/
#include "app_if.h"
/*========AAAA Include Local Header END AAAA=================================*/

/*========VVVV Typedef Definition START VVVV=================================*/
/**
 * @enum Enum
 * 列挙体の説明
 */
typedef enum {RUN_SQ_INIT, RUN_SQ_WAIT_SW2, RUN_SQ_WAIT_STARTMKR, RUN_SQ_WAIT_GOALMKR, RUN_SQ_WAIT_STOP} runsq_t;
typedef enum {MARKER_EVENT_NON, MARKER_EVENT_GOAL, MARKER_EVENT_CORNER, MARKER_EVENT_CROSS, MARKER_EVENT_OVERRUN} markerevent_t;
typedef enum {MARKER_SQ_INIT, MARKER_SQ_WAIT_MARKER, MARKER_SQ_WAIT_PASSING, MARKER_SQ_WAIT_MASK, MARKER_SQ_ERROR} markersq_t;
/*========AAAA Typedef Definition END AAAA===================================*/

/*========VVVV MACRO Definition START VVVV===================================*/
/** @def
 * マクロのコメント
 */

/*========AAAA MACRO Definition END AAAA=====================================*/

/*========VVVV GLOBAL Variable Declaration START VVVV========================*/

/*! @var   global_var1
    @brief グローバル変数の説明
*/

/*========AAAA GLOBAL Variable Declaration END AAAA==========================*/

/*========VVVV GLOBAL Function Declaration START VVVV========================*/
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

void sequencerInit(void);
void sequencerInterval_1ms(sensors_t allSensors, uint16_t maskTime1ms);
void sequencerInterval_10ms(void);
void sequencerLoop(uiin_t uii, runsq_t* runsq, markerevent_t* mkrev);
markerevent_t markerReadEvent(void);
markersq_t markerReadSequence(void);

/*========AAAA GLOBAL Function Declaration END AAAA==========================*/

#endif /* SRC_USER_APP_SEQUENCER_H_ */
