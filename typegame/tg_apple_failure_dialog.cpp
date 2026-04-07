#include "tg_apple_failure_dialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFont>

TgAppleFailureDialog::TgAppleFailureDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUI();
}

void TgAppleFailureDialog::setupUI()
{
    setModal(true);
    setWindowTitle(tr("游戏失败"));
    resize(300, 130);

    auto* mainLayout = new QVBoxLayout(this);
    m_lblMessage = new QLabel(tr("您认输吧！"), this);
    m_lblMessage->setAlignment(Qt::AlignCenter);
    m_lblMessage->setFont(QFont("Microsoft YaHei", 12, QFont::Bold));
    mainLayout->addWidget(m_lblMessage);

    auto* btnLayout = new QHBoxLayout();
    m_btnContinue = new QPushButton(tr("继续"), this);
    m_btnEnd = new QPushButton(tr("结束"), this);

    btnLayout->addStretch();
    btnLayout->addWidget(m_btnContinue);
    btnLayout->addWidget(m_btnEnd);
    mainLayout->addLayout(btnLayout);

    connect(m_btnContinue, &QPushButton::clicked, [this]() { done(TgFailureContinue); });
    connect(m_btnEnd, &QPushButton::clicked, [this]() { done(TgFailureEnd); });
}