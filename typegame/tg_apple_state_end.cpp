#include "tg_apple_state_end.h"
#include "tg_apple_logic_orchestrator.h"
TgAppleStateEnd::TgAppleStateEnd(TgAppleLogicOrchestrator& logic) : m_logic(logic) {}
void TgAppleStateEnd::onEnter() {}
void TgAppleStateEnd::onUpdate(float dt) { Q_UNUSED(dt); }
void TgAppleStateEnd::onExit() {}