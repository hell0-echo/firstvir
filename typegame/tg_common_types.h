/* ------------------------------------------------------------------
//  文件名    :   tg_common_types.h
//  创建者    :   dev@project.cn
//  创建时间  :   2024-05-20
//  功能描述  :   全局枚举、常量与跨线程数据类型定义
// ----------------------------------------------------------------*/
#ifndef _TG_COMMON_TYPES_H_
#define _TG_COMMON_TYPES_H_

#include <QChar>
#include <QRect>
#include <QVector>

enum class TgGameMode
{
    ModeNone,
    ModeApple,
    ModeSpace
};

enum class TgGameState
{
    Idle,
    Playing,
    Paused,
    End
};

constexpr int kAppleDefaultLevel = 3;
constexpr int kAppleDefaultTarget = 100;
constexpr int kAppleDefaultMaxFails = 10;
constexpr int kAppleDefaultMaxOnScreen = 5;
constexpr float kAppleFallSpeedBase = 1.0f;

#endif // _TG_COMMON_TYPES_H_