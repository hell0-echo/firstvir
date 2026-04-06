#include "tg_exit_dialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

TgExitDialog::TgExitDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUI();
}

void TgExitDialog::setupUI()
{
    setModal(true);
    setWindowTitle(tr("退出确认"));
    resize(300, 120);

    auto* mainLayout = new QVBoxLayout(this);
    m_lblMessage = new QLabel(tr("确定要退出游戏吗？"), this);
    m_lblMessage->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_lblMessage);

    auto* btnLayout = new QHBoxLayout();
    m_btnYes = new QPushButton(tr("是"), this);
    m_btnNo = new QPushButton(tr("否"), this);
    btnLayout->addStretch();
    btnLayout->addWidget(m_btnYes);
    btnLayout->addWidget(m_btnNo);
    mainLayout->addLayout(btnLayout);

    connect(m_btnYes, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_btnNo, &QPushButton::clicked, this, &QDialog::reject);
}