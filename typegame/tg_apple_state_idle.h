#ifndef _TG_APPLE_STATE_IDLE_H_
#define _TG_APPLE_STATE_IDLE_H_
#include "tg_state_interface.h"
class TgAppleLogicOrchestrator;
class TgAppleStateIdle : public TgStateInterface
{
public:
    explicit TgAppleStateIdle(TgAppleLogicOrchestrator& logic);
    void onEnter() override;
    void onUpdate(float dt) override;
    void onExit() override;
private:
    TgAppleLogicOrchestrator& m_logic;
};
#endif