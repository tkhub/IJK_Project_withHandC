/*
 * tracectrl.c
 *
 *  Created on: Nov 1, 2025
 *      Author: takap
 */


/*========VVVV Include Standard Header START VVVV============================*/
#include "orgtypedef.h"
#include <math.h>
/*========AAAA Include Standard Header END AAAA==============================*/

/*========VVVV Include Local Header START VVVV===============================*/
#include "tracectrl.h"
#include "app_if.h"
/*========AAAA Include Local Header END AAAA=================================*/

/*========VVVV Typedef Definition START VVVV=================================*/
/*========AAAA Typedef Definition END AAAA===================================*/

/*========VVVV MACRO Definition START VVVV===================================*/
/*========AAAA MACRO Definition END AAAA=====================================*/

/*========VVVV GLOBAL Variable Definition START VVVV=========================*/
/*========AAAA GLOBAL Variable Definition END AAAA===========================*/

/*========VVVV Private Variable Definition START VVVV========================*/
/*! @var   global_var1
    @brief グローバル変数の説明
*/
static float speed;
static float linepos_last;
static float linedpos_last;
static float kp;
static float kd;
static float kdd;
static float steering;
static float motorLpwr, motorRpwr;

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
void divider(const float acc, const float str, float* l, float* r);
/*========AAAA Private Function Prototype Declaration END AAAA===============*/

/*========VVVV GLOBAL Function Definition START VVVV=========================*/
void tracectrlInit(void) {
    speed = 0.0F;
    steering = 0.0F;
    kp = 0.015F;
    kd = 0.2F;
    kdd = 0.075F;
}

void traceCtrInterval_1ms(sensors_t allSensors, drives_t drives) {
    steering = allSensors.linesensor.position * kp + (allSensors.linesensor.position - linepos_last) * kd + ((allSensors.linesensor.position - linepos_last) - linedpos_last) * kdd;
    linedpos_last = allSensors.linesensor.position - linepos_last;
    linepos_last = allSensors.linesensor.position;

    divider(drives.powerLR - fabs(drives.breakGain * allSensors.linesensor.position) , steering, &motorLpwr, &motorRpwr);
    if ((-0.05F < drives.powerLR) && (drives.powerLR < 0.05F)) {
        motorsDrive(0, 0);
    }
    else {
        motorsDrive(motorLpwr, motorRpwr);
    }

//    divider(0.4, steering, &motorLpwr, &motorRpwr);

}
/*========AAAA GLOBAL Function Definition END AAAA===========================*/

/*========VVVV Private Function Definition START VVVV========================*/
void divider(const float acc, const float str, float* l, float* r) {
    *l = acc - str;
    *r = acc + str;
}
/*========AAAA Private Function Definition END AAAA==========================*/

