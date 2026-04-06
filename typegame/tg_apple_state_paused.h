#ifndef _TG_APPLE_STATE_PAUSED_H_
#define _TG_APPLE_STATE_PAUSED_H_
#include "tg_state_interface.h"
class TgAppleLogicOrchestrator;
class TgAppleStatePaused : public TgStateInterface
{
public:
    explicit TgAppleStatePaused(TgAppleLogicOrchestrator& logic);
    void onEnter() override;
    void onUpdate(float dt) override;
    void onExit() override;
private:
    TgAppleLogicOrchestrator& m_logic;
};
#endif