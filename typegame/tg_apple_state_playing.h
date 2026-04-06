#ifndef _TG_APPLE_STATE_PLAYING_H_
#define _TG_APPLE_STATE_PLAYING_H_
#include "tg_state_interface.h"
class TgAppleLogicOrchestrator;
class TgAppleStatePlaying : public TgStateInterface
{
public:
    explicit TgAppleStatePlaying(TgAppleLogicOrchestrator& logic);
    void onEnter() override;
    void onUpdate(float dt) override;
    void onExit() override;
private:
    TgAppleLogicOrchestrator& m_logic;
    float m_spawnAcc = 0.0f;
};
#endif