#include "tg_apple_state_paused.h"
#include "tg_apple_logic_orchestrator.h"
TgAppleStatePaused::TgAppleStatePaused(TgAppleLogicOrchestrator& logic) : m_logic(logic) {}
void TgAppleStatePaused::onEnter() {}
void TgAppleStatePaused::onUpdate(float dt) { Q_UNUSED(dt); }
void TgAppleStatePaused::onExit() {}
