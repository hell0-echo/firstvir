#include "tg_main_menu.h"
#include "tg_custom_button.h"
#include <QVBoxLayout>
#include <QPainter>

TgMainMenu::TgMainMenu(QWidget* parent)
    : QWidget(parent)
    , m_btnAppleGame(nullptr)
    , m_btnSpaceGame(nullptr)
    , m_btnSettings(nullptr)
    , m_btnExit(nullptr)
{
    setAutoFillBackground(false);
    setupUI();
}

void TgMainMenu::setupUI()
{
    auto* layout = new QVBoxLayout(this);
    layout->setSpacing(20);
    layout->addStretch(2);

    // Create buttons
    m_btnAppleGame = new TgCustomButton(tr("拯救苹果"), this);
    m_btnSpaceGame = new TgCustomButton(tr("太空大战"), this);
    m_btnSettings = new TgCustomButton(tr("设置"), this);
    m_btnExit = new TgCustomButton(tr("退出"), this);

    // Set button size
    QSize btnSize(200, 60);
    m_btnAppleGame->setFixedSize(btnSize);
    m_btnSpaceGame->setFixedSize(btnSize);
    m_btnSettings->setFixedSize(btnSize);
    m_btnExit->setFixedSize(btnSize);

    // Set font
    QFont btnFont("Microsoft YaHei", 14, QFont::Bold);
    m_btnAppleGame->setFont(btnFont);
    m_btnSpaceGame->setFont(btnFont);
    m_btnSettings->setFont(btnFont);
    m_btnExit->setFont(btnFont);

    // Connect signals
    connect(m_btnAppleGame, &QPushButton::clicked, this, &TgMainMenu::onAppleGameClicked);
    connect(m_btnSpaceGame, &QPushButton::clicked, this, &TgMainMenu::onSpaceGameClicked);
    connect(m_btnSettings, &QPushButton::clicked, this, &TgMainMenu::onSettingsClicked);
    connect(m_btnExit, &QPushButton::clicked, this, &TgMainMenu::onExitClicked);

    // Add to layout
    layout->addWidget(m_btnAppleGame, 0, Qt::AlignHCenter);
    layout->addWidget(m_btnSpaceGame, 0, Qt::AlignHCenter);
    layout->addWidget(m_btnSettings, 0, Qt::AlignHCenter);
    layout->addWidget(m_btnExit, 0, Qt::AlignHCenter);
    layout->addStretch(1);
}

void TgMainMenu::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    if (!m_background.isNull())
    {
        painter.drawPixmap(rect(), m_background);
    }
    else
    {
        // Fallback gradient background
        QLinearGradient gradient(0, 0, 0, height());
        gradient.setColorAt(0, QColor(20, 40, 80));
        gradient.setColorAt(1, QColor(10, 20, 40));
        painter.fillRect(rect(), gradient);
    }
}

void TgMainMenu::onAppleGameClicked()
{
    emit gameModeSelected(TgGameMode::ModeApple);
}

void TgMainMenu::onSpaceGameClicked()
{
    emit gameModeSelected(TgGameMode::ModeSpace);
}

void TgMainMenu::onSettingsClicked()
{
    emit settingsRequested();
}

void TgMainMenu::onExitClicked()
{
    emit exitRequested();
}