// tg_confirm_dialog.cpp
#include "tg_confirm_dialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

TgConfirmDialog::TgConfirmDialog(const QString& message, QWidget* parent)
    : QDialog(parent)
{
    setModal(true);
    setWindowTitle(tr("确认操作"));
    resize(300, 120);
    auto* layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel(message, this));
    auto* btnLayout = new QHBoxLayout();
    m_btnYes = new QPushButton(tr("是"), this);
    m_btnNo = new QPushButton(tr("否"), this);
    btnLayout->addStretch(); btnLayout->addWidget(m_btnYes); btnLayout->addWidget(m_btnNo);
    layout->addLayout(btnLayout);
    connect(m_btnYes, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_btnNo, &QPushButton::clicked, this, &QDialog::reject);
}