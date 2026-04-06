/* ------------------------------------------------------------------
//  文件名    :   tg_main_window.h
//  创建者    :   dev@project.cn
//  创建时间  :   2024-05-20
//  功能描述  :   主窗口控制层：UI路由、状态同步、弹窗交互、配置/音频管理
// ----------------------------------------------------------------*/
#ifndef _TG_MAIN_WINDOW_H_
#define _TG_MAIN_WINDOW_H_

#include <QWidget>
#include <QObject>
#include <QKeyEvent>
#include <QStackedWidget>
#include "tg_common_types.h"
#include "tg_thread_sync.h"
#include "tg_game_loop.h"
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

class TgMainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TgMainWindow(QWidget* parent = nullptr);
    ~TgMainWindow() override;

    TgMainWindow(const TgMainWindow&) = delete;
    TgMainWindow& operator=(const TgMainWindow&) = delete;

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onGameModeSelected(TgGameMode mode);
    void onLogicStateChanged(TgGameState state);
    void showSettings();
    void showExitConfirm();
    void onGameEnd();

private:
    void setupUI();
    void setupConnections();
    void initializeAppleGame();
    void showGameScene(TgGameMode mode);
    void showMainMenu();
    void applyNewConfig(const TgAppleConfig& config);
    void loadConfigAndApply();

    TgThreadSync m_sync;
    TgGameLoop m_loop;
    QStackedWidget* m_stackedWidget;
    TgMainMenu* m_mainMenu;
    QWidget* m_gameScene;

    TgAppleLogicOrchestrator* m_appleLogic;
    TgGameCanvas* m_appleCanvas;
    TgAppleBottomToolbar* m_toolbar;
    TgAppleStateIdle* m_appleIdleState;
    TgAppleStatePlaying* m_applePlayingState;
    TgAppleStatePaused* m_applePausedState;
    TgAppleStateEnd* m_appleEndState;

    TgConfigManager* m_configMgr;
    TgAudioManager* m_audioMgr;
    TgAppleConfig m_currentConfig;
    TgGameMode m_currentMode;
    bool m_isGameRunning;
};

#endif // _TG_MAIN_WINDOW_H_