#ifndef _TG_MAIN_WINDOW_H_
#define _TG_MAIN_WINDOW_H_

#include <QWidget>
#include <QStackedWidget>
#include <QTimer> 
#include "tg_common_types.h"
#include "tg_apple_logic_orchestrator.h"
#include "tg_game_canvas.h"
#include "tg_apple_state_idle.h"
#include "tg_apple_state_playing.h"
#include "tg_apple_state_paused.h"
#include "tg_apple_state_end.h"
#include "tg_main_menu.h"
#include "tg_apple_bottom_toolbar.h"
#include "tg_config_manager.h"
#include "tg_audio_manager.h"
#include "tg_space_game_logic.h"
#include "tg_space_canvas.h"

class TgMainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit TgMainWindow(QWidget* parent = nullptr);
    ~TgMainWindow() override;

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onGameModeSelected(TgGameMode mode);
    void onLogicStateChanged(TgGameState state);
    void showSettings();
    void showExitConfirm();
    void onGameEnd();
    void onGameTick();

private:
    void setupUI();
    void setupConnections();
    void initializeAppleGame();
    void initializeSpaceGame();
    void showGameScene(TgGameMode mode);
    void showMainMenu();
    void applyNewConfig(const TgAppleConfig& config);
    void loadConfigAndApply();

    QTimer m_gameTimer; 
    QStackedWidget* m_stackedWidget;
    TgMainMenu* m_mainMenu;
    QWidget* m_gameScene;

    TgAppleLogicOrchestrator* m_appleLogic = nullptr;
    TgGameCanvas* m_appleCanvas = nullptr;
    TgAppleBottomToolbar* m_toolbar = nullptr;
    TgAppleStateIdle* m_appleIdleState = nullptr;
    TgAppleStatePlaying* m_applePlayingState = nullptr;
    TgAppleStatePaused* m_applePausedState = nullptr;
    TgAppleStateEnd* m_appleEndState = nullptr;

    TgConfigManager* m_configMgr;
    TgAudioManager* m_audioMgr;
    TgAppleConfig m_currentConfig;

    TgGameMode m_currentMode = TgGameMode::ModeNone;
    bool m_isGameRunning = false;

    TgSpaceGameLogic* m_spaceLogic = nullptr;
    TgSpaceCanvas* m_spaceCanvas = nullptr;
    bool m_isSpaceInitialized = false;
    void onSpaceGameOver();
};
#endif // _TG_MAIN_WINDOW_H_