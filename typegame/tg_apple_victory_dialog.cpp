#include "tg_apple_victory_dialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFont>

TgAppleVictoryDialog::TgAppleVictoryDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUI();
}

void TgAppleVictoryDialog::setupUI()
{
    setModal(true);
    setWindowTitle(tr("游戏胜利"));
    resize(350, 150);

    auto* mainLayout = new QVBoxLayout(this);
    m_lblMessage = new QLabel(tr("恭喜，您通过了！"), this);
    m_lblMessage->setAlignment(Qt::AlignCenter);
    m_lblMessage->setFont(QFont("Microsoft YaHei", 12, QFont::Bold));
    mainLayout->addWidget(m_lblMessage);

    auto* btnLayout = new QHBoxLayout();
    m_btnContinue = new QPushButton(tr("继续"), this);
    m_btnNextLevel = new QPushButton(tr("下一关"), this);
    m_btnEnd = new QPushButton(tr("结束"), this);

    btnLayout->addWidget(m_btnContinue);
    btnLayout->addWidget(m_btnNextLevel);
    btnLayout->addWidget(m_btnEnd);
    mainLayout->addLayout(btnLayout);

    connect(m_btnContinue, &QPushButton::clicked, [this]() { done(TgVictoryContinue); });
    connect(m_btnNextLevel, &QPushButton::clicked, [this]() { done(TgVictoryNextLevel); });
    connect(m_btnEnd, &QPushButton::clicked, [this]() { done(TgVictoryEnd); });
}