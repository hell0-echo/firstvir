#ifndef _TG_APPLE_STATE_END_H_
#define _TG_APPLE_STATE_END_H_
#include "tg_state_interface.h"
class TgAppleLogicOrchestrator;
class TgAppleStateEnd : public TgStateInterface
{
public:
    explicit TgAppleStateEnd(TgAppleLogicOrchestrator& logic);
    void onEnter() override;
    void onUpdate(float dt) override;
    void onExit() override;
private:
    TgAppleLogicOrchestrator& m_logic;
};
#endif