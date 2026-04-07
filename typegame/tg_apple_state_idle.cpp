#include "tg_apple_state_idle.h"
#include "tg_apple_logic_orchestrator.h"
TgAppleStateIdle::TgAppleStateIdle(TgAppleLogicOrchestrator& logic) : m_logic(logic) {}
void TgAppleStateIdle::onEnter() { m_logic.reset(); }
void TgAppleStateIdle::onUpdate(float dt) { Q_UNUSED(dt); }
void TgAppleStateIdle::onExit() {}
