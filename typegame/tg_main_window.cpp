/* ------------------------------------------------------------------
//  文件名    :   tg_main_window.cpp
//  创建者    :   dev@project.cn
//  创建时间  :   2024-05-20
//  功能描述  :   主窗口控制层实现
// ----------------------------------------------------------------*/
#include "tg_main_window.h"

#include <QVBoxLayout>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QDir>
#include <QDialog> // 修复 QDialog 不完整类型错误

// 弹窗头文件必须包含，否则枚举与类型未定义
#include "tg_settings_dialog.h"
#include "tg_confirm_dialog.h"
#include "tg_exit_dialog.h"
#include "tg_apple_victory_dialog.h"
#include "tg_apple_failure_dialog.h"

TgMainWindow::TgMainWindow(QWidget* parent)
    : QWidget(parent)
    , m_sync()
    , m_loop()
    , m_stackedWidget(new QStackedWidget(this))
    , m_mainMenu(new TgMainMenu(this))
    , m_gameScene(new QWidget(this))
    , m_appleLogic(nullptr)
    , m_appleCanvas(nullptr)
    , m_toolbar(nullptr)
    , m_appleIdleState(nullptr)
    , m_applePlayingState(nullptr)
    , m_applePausedState(nullptr)
    , m_appleEndState(nullptr)
    , m_configMgr(new TgConfigManager(QDir::currentPath() + "/config.ini"))
    , m_audioMgr(new TgAudioManager(this))
    , m_currentMode(TgGameMode::ModeNone)
    , m_isGameRunning(false)
{
    setupUI();
    loadConfigAndApply();
    setupConnections();
}

TgMainWindow::~TgMainWindow()
{
    m_configMgr->saveAppleConfig(m_currentConfig);
}

void TgMainWindow::setupUI()
{
    setWindowTitle(tr("TypeGame"));
    resize(1000, 700);
    setAttribute(Qt::WA_DeleteOnClose);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    m_stackedWidget->addWidget(m_mainMenu);
    m_stackedWidget->addWidget(m_gameScene);
    m_stackedWidget->setCurrentIndex(0);

    mainLayout->addWidget(m_stackedWidget);
}

void TgMainWindow::setupConnections()
{
    connect(m_mainMenu, &TgMainMenu::gameModeSelected, this, &TgMainWindow::onGameModeSelected);
    connect(m_mainMenu, &TgMainMenu::exitRequested, this, &TgMainWindow::showExitConfirm);
    connect(m_mainMenu, &TgMainMenu::settingsRequested, this, &TgMainWindow::showSettings);

    connect(&m_loop, &TgGameLoop::tick, this, [this]() {
        if (m_isGameRunning && m_appleLogic != nullptr) {
            m_appleLogic->step(0.016f);
        }
        }, Qt::DirectConnection);
}

void TgMainWindow::initializeAppleGame()
{
    if (m_appleLogic != nullptr) return;

    auto* gameLayout = new QVBoxLayout(m_gameScene);
    gameLayout->setContentsMargins(0, 0, 0, 0);
    gameLayout->setSpacing(0);

    m_appleCanvas = new TgGameCanvas(m_sync, m_gameScene);
    gameLayout->addWidget(m_appleCanvas, 1);

    m_toolbar = new TgAppleBottomToolbar(m_gameScene);
    m_toolbar->setVisible(false);
    gameLayout->addWidget(m_toolbar);

    m_appleLogic = new TgAppleLogicOrchestrator(m_sync);
    m_appleIdleState = new TgAppleStateIdle(*m_appleLogic);
    m_applePlayingState = new TgAppleStatePlaying(*m_appleLogic);
    m_applePausedState = new TgAppleStatePaused(*m_appleLogic);
    m_appleEndState = new TgAppleStateEnd(*m_appleLogic);

    m_appleLogic->setState(m_appleIdleState);

    connect(m_appleLogic, &TgAppleLogicOrchestrator::stateChanged, this, &TgMainWindow::onLogicStateChanged);
    connect(m_appleLogic, &TgAppleLogicOrchestrator::victoryTriggered, this, &TgMainWindow::onGameEnd);
    connect(m_appleLogic, &TgAppleLogicOrchestrator::defeatTriggered, this, &TgMainWindow::onGameEnd);

    connect(m_toolbar, &TgAppleBottomToolbar::pauseClicked, this, [this]() {
        if (m_appleLogic == nullptr) return;
        if (m_appleLogic->getCurrentState() == TgGameState::Playing) {
            m_appleLogic->setState(m_applePausedState);
        }
        else {
            m_appleLogic->setState(m_applePlayingState);
        }
        });

    connect(m_toolbar, &TgAppleBottomToolbar::startClicked, this, [this]() {
        if (m_appleLogic == nullptr) return;
        m_appleLogic->reset();
        m_appleLogic->setState(m_applePlayingState);
        });

    connect(m_toolbar, &TgAppleBottomToolbar::endClicked, this, [this]() {
        if (m_appleLogic == nullptr) return;
        m_appleLogic->setState(m_appleEndState);
        });

    connect(m_toolbar, &TgAppleBottomToolbar::settingsClicked, this, &TgMainWindow::showSettings);
    connect(m_toolbar, &TgAppleBottomToolbar::exitClicked, this, &TgMainWindow::showExitConfirm);
}

void TgMainWindow::showGameScene(TgGameMode mode)
{
    if (mode == TgGameMode::ModeApple) {
        initializeAppleGame();
        m_isGameRunning = true;
        m_loop.start();
        m_toolbar->setVisible(true);
        m_stackedWidget->setCurrentIndex(1);
    }
    else {
        m_isGameRunning = true;
        m_loop.start();
        m_stackedWidget->setCurrentIndex(1);
    }
}

void TgMainWindow::showMainMenu()
{
    m_isGameRunning = false;
    m_loop.stop();
    if (m_appleLogic != nullptr) {
        m_appleLogic->setState(m_appleIdleState);
    }
    m_stackedWidget->setCurrentIndex(0);
}

void TgMainWindow::onGameModeSelected(TgGameMode mode)
{
    m_currentMode = mode;
    showGameScene(mode);
}

void TgMainWindow::onLogicStateChanged(TgGameState state)
{
    if (m_toolbar != nullptr) {
        m_toolbar->updateButtonStates(state);
    }
}

void TgMainWindow::keyPressEvent(QKeyEvent* event)
{
    if (m_currentMode == TgGameMode::ModeApple && m_appleLogic != nullptr && m_isGameRunning) {
        if (event->key() == Qt::Key_Escape) {
            showMainMenu();
            return;
        }

        if (event->key() == Qt::Key_Space) {
            if (m_appleLogic->isVictory() || m_appleLogic->isDefeat()) return;
            if (m_appleLogic->getCurrentState() == TgGameState::Playing) {
                m_appleLogic->setState(m_applePausedState);
            }
            else {
                m_appleLogic->setState(m_applePlayingState);
            }
            return;
        }

        QString keyText = event->text();
        if (!keyText.isEmpty()) {
            QChar inputChar = keyText.at(0);
            if (inputChar.isLetter()) {
                m_appleLogic->handleInput(inputChar.toUpper());
                return;
            }
        }
    }
    QWidget::keyPressEvent(event);
}

void TgMainWindow::showSettings()
{
    TgSettingsDialog dialog(m_currentConfig, this);
    if (dialog.exec() == QDialog::Accepted) {
        TgConfirmDialog confirm(tr("设置已更改，是否立即生效？"), this);
        if (confirm.exec() == QDialog::Accepted) {
            applyNewConfig(dialog.getConfig());
        }
    }
}

void TgMainWindow::showExitConfirm()
{
    TgExitDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        close();
    }
}

void TgMainWindow::onGameEnd()
{
    if (m_appleLogic == nullptr) return;

    if (m_appleLogic->isVictory()) {
        TgAppleVictoryDialog victoryDialog(this);
        int result = victoryDialog.exec();
        if (result == TgVictoryContinue) {
            m_appleLogic->reset();
            m_appleLogic->setState(m_applePlayingState);
        }
        else if (result == TgVictoryNextLevel) {
            m_currentConfig.level = qMin(9, m_currentConfig.level + 1);
            applyNewConfig(m_currentConfig);
            m_appleLogic->reset();
            m_appleLogic->setState(m_applePlayingState);
        }
        else {
            showMainMenu();
        }
    }
    else {
        TgAppleFailureDialog failureDialog(this);
        int result = failureDialog.exec();
        if (result == TgFailureContinue) {
            m_appleLogic->reset();
            m_appleLogic->setState(m_applePlayingState);
        }
        else {
            showMainMenu();
        }
    }
}

void TgMainWindow::applyNewConfig(const TgAppleConfig& config)
{
    m_currentConfig = config;
    m_audioMgr->setSoundEnabled(m_currentConfig.soundEnabled);
    m_configMgr->saveAppleConfig(m_currentConfig);

    if (m_appleLogic != nullptr) {
        m_appleLogic->applyConfig(m_currentConfig);
    }
}

void TgMainWindow::loadConfigAndApply()
{
    m_configMgr->loadAppleConfig(m_currentConfig);
    m_audioMgr->setSoundEnabled(m_currentConfig.soundEnabled);
}

void TgMainWindow::closeEvent(QCloseEvent* event)
{
    m_configMgr->saveAppleConfig(m_currentConfig);
    QWidget::closeEvent(event);
}