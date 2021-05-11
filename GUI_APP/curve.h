/**
 * @file curve.h
 * @brief
 * @author  xiaowine (xiaowine@sina.cn)
 * @version 01.00
 * @date    2020-10-12
 *
 * @copyright Copyright (c) {2020}  xiaowine
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2020-10-12 <td>1.0     <td>wangh     <td>内容
 * </table>
 * ******************************************************************
 * *                   .::::
 * *                 .::::::::
 * *                ::::::::::
 * *             ..:::::::::::
 * *          '::::::::::::
 * *            .:::::::::
 * *       '::::::::::::::..        女神助攻,流量冲天
 * *            ..::::::::::::.     永不宕机,代码无bug
 * *          ``:::::::::::::::
 * *           ::::``:::::::::'        .:::
 * *          ::::'   ':::::'       .::::::::
 * *        .::::'      ::::     .:::::::'::::
 * *       .:::'       :::::  .:::::::::' ':::::
 * *      .::'        :::::.:::::::::'      ':::::
 * *     .::'         ::::::::::::::'         ``::::
 * * ...:::           ::::::::::::'              ``::
 * *```` ':.          ':::::::::'                  ::::.
 * *                   '.:::::'                    ':'````.
 * ******************************************************************
 */
#ifndef __CURVE_H_
#define __CURVE_H_
#include "string.h"
#include "sys.h"

#define TimerPeriod 1200  // T*100ms

#define CURVE_FLASH_ADDR 0
#define CURVE_VP_ADDR 0x1000
#define CURVE_LEN 0x1000

// curve0---
#define Curve0Point 0x0300
#define Curve0Len 0x0301
#define Curve0Start 0x1000
#define Curve0End 0x17FE
// curve1---
#define Curve1Point 0x0302
#define Curve1Len 0x0303
#define Curve1Start 0x1800
#define Curve1End 0x1FFE

// #define Curve0Addr 0x5004  // 1个
// #define Curve1Addr 0x5005  // 1个
#define Curve0Data 0xa0a3  // 1个
#define Curve1Data 0xa0a4  // 1个

#define CurveMAX 0x07FE
#define Curveffset 0x1000
#define Curvetemp 0x5000  // 4个
#define Curveflag 0x5002  // 1个

// Curvedrag---
#define CurveManual 0x5008
#define CurveManualBak 0x5009
#define CurveDrager 0x500A  // 10个
#define DragerX 0x5011
#define DragerLeft 143
#define DragerRight 650

void curveInit(void);
void curveProcess(void);
void dragCuave(void);
void curveClearHandle(void);

#endif
