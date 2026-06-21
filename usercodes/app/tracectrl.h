/*
 * tracectrl.h
 *
 *  Created on: Nov 1, 2025
 *      Author: takap
 */

#ifndef SRC_USER_APP_TRACECTRL_TRACECTRL_H_
#define SRC_USER_APP_TRACECTRL_TRACECTRL_H_

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

void tracectrlInit(void);
void traceCtrInterval_1ms(sensors_t allSensors, drives_t drives);

/*========AAAA GLOBAL Function Declaration END AAAA==========================*/

#endif /* SRC_USER_APP_TRACECTRL_TRACECTRL_H_ */
