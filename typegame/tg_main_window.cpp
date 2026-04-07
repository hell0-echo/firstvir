
#include "tg_main_window.h"

#include <QVBoxLayout>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QDir>
#include <QDialog> 
#include <QMessageBox> 

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
    , m_spaceLogic(nullptr)
    , m_spaceCanvas(nullptr)
    , m_isSpaceInitialized(false)
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








// 输入路由补充
void TgMainWindow::keyPressEvent(QKeyEvent* event)
{
    if (m_isGameRunning) {
        if (event->key() == Qt::Key_Escape) {
            showMainMenu();
            return;
        }
        if (m_currentMode == TgGameMode::ModeApple && m_appleLogic) {
            if (event->key() == Qt::Key_Space) { /* 暂停逻辑... */ }
            QString keyText = event->text();
            if (!keyText.isEmpty() && keyText.at(0).isLetter()) {
                m_appleLogic->handleInput(keyText.at(0).toUpper());
                return;
            }
        }
        else if (m_currentMode == TgGameMode::ModeSpace && m_spaceLogic) {
            QString keyText = event->text();
            if (!keyText.isEmpty() && keyText.at(0).isLetter()) {
                m_spaceLogic->handleInput(keyText.at(0).toUpper());
                return;
            }
        }
    }
    QWidget::keyPressEvent(event);
}

// 游戏结束处理
void TgMainWindow::onSpaceGameOver()
{
    QMessageBox::information(this, tr("游戏结束"), tr("飞船已损毁！最终得分: %1").arg(m_spaceLogic ? m_spaceLogic->getScore() : 0));
    showMainMenu();
}


void TgMainWindow::initializeSpaceGame()
{
    if (m_spaceLogic != nullptr) return;  // 避免重复初始化

    auto* layout = new QVBoxLayout(m_gameScene);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // 创建太空画布（需先创建 tg_space_canvas.h/cpp，见下方说明）
    m_spaceCanvas = new TgSpaceCanvas(m_sync, m_gameScene);
    layout->addWidget(m_spaceCanvas, 1);

    // 创建逻辑实例
    m_spaceLogic = new TgSpaceGameLogic(m_sync);
    connect(m_spaceLogic, &TgSpaceGameLogic::gameOver, this, [this]() {
        QMessageBox::information(this, tr("游戏结束"),
            tr("飞船已损毁！最终得分：%1").arg(m_spaceLogic->getScore()));
        showMainMenu();
        });
}

// 确保只保留一份 setupConnections
void TgMainWindow::setupConnections()
{
    connect(m_mainMenu, &TgMainMenu::gameModeSelected, this, &TgMainWindow::onGameModeSelected);
    connect(m_mainMenu, &TgMainMenu::exitRequested, this, &TgMainWindow::showExitConfirm);
    connect(m_mainMenu, &TgMainMenu::settingsRequested, this, &TgMainWindow::showSettings);

    connect(&m_loop, &TgGameLoop::tick, this, [this]() {
        if (!m_isGameRunning) return;
        if (m_currentMode == TgGameMode::ModeApple && m_appleLogic) {
            m_appleLogic->step(0.016f);
        }
        else if (m_currentMode == TgGameMode::ModeSpace && m_spaceLogic) {
            m_spaceLogic->step(0.016f);
        }
        }, Qt::DirectConnection);
}

// 确保只保留一份 showGameScene
void TgMainWindow::showGameScene(TgGameMode mode)
{
    m_isGameRunning = true;
    m_currentMode = mode;

    if (mode == TgGameMode::ModeApple) {
        initializeAppleGame();
        if (m_appleCanvas) m_appleCanvas->setVisible(true);
        if (m_toolbar) m_toolbar->setVisible(true);
        if (m_spaceCanvas) m_spaceCanvas->setVisible(false);
    }
    else if (mode == TgGameMode::ModeSpace) {
        initializeSpaceGame();
        if (m_appleCanvas) m_appleCanvas->setVisible(false);
        if (m_toolbar) m_toolbar->setVisible(false);
        if (m_spaceCanvas) m_spaceCanvas->setVisible(true);
    }

    m_stackedWidget->setCurrentIndex(1);
    m_loop.start();
}

void TgMainWindow::showMainMenu()
{
    m_isGameRunning = false;
    m_loop.stop();
    if (m_appleLogic) m_appleLogic->setState(m_appleIdleState);
    m_stackedWidget->setCurrentIndex(0);
}