/* ------------------------------------------------------------------
//  文件名    :   tg_state_interface.h
//  创建者    :   dev@project.cn
//  创建时间  :   2024-05-20
//  功能描述  :   游戏状态机纯虚接口
// ----------------------------------------------------------------*/
#ifndef _TG_STATE_INTERFACE_H_
#define _TG_STATE_INTERFACE_H_

class TgStateInterface
{
public:
    virtual ~TgStateInterface() = default;
    virtual void onEnter() = 0;
    virtual void onUpdate(float dt) = 0;
    virtual void onExit() = 0;
};

#endif // _TG_STATE_INTERFACE_H_