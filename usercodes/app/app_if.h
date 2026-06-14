/**
 * @file test.h
 * @brief 簡単な説明  
 * @author takap
 * @date Nov 30, 2025
 * @Version 0.00
 */

#ifndef __APP_IF_H__
#define __APP_IF_H__

#ifdef __cplusplus
extern "C" {
#endif

/*========VVVV Include Standard Header START VVVV============================*/
/*========AAAA Include Standard Header END AAAA==============================*/

/*========VVVV Include Local Header START VVVV===============================*/
#include <sac.h>

/*========AAAA Include Local Header END AAAA=================================*/

/*========VVVV Typedef Definition START VVVV=================================*/
/*===========================================================================*/
/*                              sensors                                      */
/*===========================================================================*/

/**
 * @typedef linesensors_t
 * @brief 匿名構造体を linesensors_t した型
 * ラインセンサの情報
 */
typedef struct {
    float position; /**< センサからのズレ（左ズレマイナス、右ズレプラス） */
    uint8_t binaries;
} linesensors_t ;

/**
 * @typedef tire_t
 * @brief 匿名構造体を tire_t した型
 * タイヤの回転情報を集約
 */
typedef struct {
     float speed;
     float odd;
} tire_t;


/**
 * @typedef tire_t
 * @brief 匿名構造体を tire_t した型
 * タイヤの回転情報を集約
 */
typedef struct {
    tire_t  left;
    tire_t  right;
    float   speed;
    float   omega;
} odometry_t;

/**
 * @typedef battery_t
 * @brief 匿名構造体を battery_t した型
 * バッテリーの情報を集約
 */
typedef struct {
    float           voltage;
    batterystatus_t status;
} battery_t;


/**
 * @typedef battery_t
 * @brief 匿名構造体を battery_t した型
 * バッテリーの情報を集約
 */
typedef struct {
    linesensors_t   linesensor;     /**<ラインセンサ情報 */
    uint8_t         markerFlags;    /**<マーカー情報 */
    odometry_t      odometry;
    battery_t       battery;
} sensors_t;


/*===========================================================================*/
/*                              drives                                       */
/*===========================================================================*/
typedef struct {
   float power;
} drive_t;

typedef struct {
    drive_t         left;
    drive_t         right;
    float           powerLR;
} drives_t;

/*===========================================================================*/
/*                              ui                                           */
/*===========================================================================*/

typedef struct {
    buzzerSchedule_t    bzschedule;
} uiout_t;


typedef struct {
    uiswevent_t         uiswevent;
} uiin_t;


/*========VVVV Typedef Definition START VVVV=================================*/
/**
 * @brief ABC_t 型の列挙体
 */
/**
 * typedef enum {
 *  A,  //! Aの説明
 *  B,  //! Bの説明
 *  C   //! Cの説明
 * } ABC_t;
*/

/**
 * @brief DEF_t 型の構造体
 */
/**
 * typedef struct {
 *  int D,  //!  Dの説明
 *  int E,  //!  Eの説明
 *  int F   //!  Fの説明
 * } DEF_t;
*/

/*========AAAA Typedef Definition END AAAA===================================*/

/*========VVVV MACRO Definition START VVVV===================================*/
/** 
 * @brief マクロ MAX_NANK の説明
 */
/* # define MAX_NANKA 256 */

/*========AAAA MACRO Definition END AAAA=====================================*/

/*========VVVV GLOBAL Variable Declaration START VVVV========================*/
/**
 * @brief グローバル変数 lobal_var の説明  
*/
/**
 * extern int global_var;
*/

/*========AAAA GLOBAL Variable Declaration END AAAA==========================*/

/*========VVVV GLOBAL Function Declaration START VVVV========================*/
/**
 * @brief テスト関数
 * @param [in/out] buffer バッファ
 * @param [in] size バッファのサイズ
 * @return 戻り値の説明
 * @details 詳細な説明
 */
/**
 * int testFunc(int* buffer, int size)
*/

/*========AAAA GLOBAL Function Declaration END AAAA==========================*/

#ifdef __cplusplus
}
#endif

#endif /* __APP_IF_H__ */
