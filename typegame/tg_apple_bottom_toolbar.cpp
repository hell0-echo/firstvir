// tg_apple_bottom_toolbar.cpp
#include "tg_apple_bottom_toolbar.h"
#include <QHBoxLayout>

TgAppleBottomToolbar::TgAppleBottomToolbar(QWidget* parent)
    : QWidget(parent)
{
    setFixedHeight(60);
    setStyleSheet("background-color: #2c3e50;");
    setupUI();
}

void TgAppleBottomToolbar::setupUI()
{
    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 5, 10, 5);
    layout->setSpacing(10);

    m_btnPause = new TgCustomButton(tr("暂停"), this);
    m_btnStart = new TgCustomButton(tr("开始"), this);
    m_btnEnd = new TgCustomButton(tr("结束"), this);
    m_btnSettings = new TgCustomButton(tr("设置"), this);
    m_btnExit = new TgCustomButton(tr("退出"), this);

    QSize btnSize(100, 40);
    m_btnPause->setFixedSize(btnSize);
    m_btnStart->setFixedSize(btnSize);
    m_btnEnd->setFixedSize(btnSize);
    m_btnSettings->setFixedSize(btnSize);
    m_btnExit->setFixedSize(btnSize);

    QFont font("Microsoft YaHei", 10);
    m_btnPause->setFont(font);
    m_btnStart->setFont(font);
    m_btnEnd->setFont(font);
    m_btnSettings->setFont(font);
    m_btnExit->setFont(font);

    connect(m_btnPause, &QPushButton::clicked, this, &TgAppleBottomToolbar::pauseClicked);
    connect(m_btnStart, &QPushButton::clicked, this, &TgAppleBottomToolbar::startClicked);
    connect(m_btnEnd, &QPushButton::clicked, this, &TgAppleBottomToolbar::endClicked);
    connect(m_btnSettings, &QPushButton::clicked, this, &TgAppleBottomToolbar::settingsClicked);
    connect(m_btnExit, &QPushButton::clicked, this, &TgAppleBottomToolbar::exitClicked);

    layout->addWidget(m_btnPause);
    layout->addWidget(m_btnStart);
    layout->addWidget(m_btnEnd);
    layout->addWidget(m_btnSettings);
    layout->addWidget(m_btnExit);
    layout->addStretch(); // 推至左下角
}

void TgAppleBottomToolbar::updateButtonStates(TgGameState state)
{
    switch (state)
    {
    case TgGameState::Idle:
        m_btnPause->setEnabled(false);
        m_btnPause->setText(tr("暂停"));
        m_btnStart->setEnabled(true);
        m_btnEnd->setEnabled(false);
        break;
    case TgGameState::Playing:
        m_btnPause->setEnabled(true);
        m_btnPause->setText(tr("暂停"));
        m_btnStart->setEnabled(false);
        m_btnEnd->setEnabled(true);
        break;
    case TgGameState::Paused:
        m_btnPause->setEnabled(true);
        m_btnPause->setText(tr("继续"));
        m_btnStart->setEnabled(false);
        m_btnEnd->setEnabled(true);
        break;
    case TgGameState::End:
        m_btnPause->setEnabled(false);
        m_btnPause->setText(tr("暂停"));
        m_btnStart->setEnabled(true);
        m_btnEnd->setEnabled(false);
        break;
    }
    m_btnSettings->setEnabled(true);
    m_btnExit->setEnabled(true);
}