// tg_settings_dialog.cpp
#include "tg_settings_dialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QCheckBox>
#include <QPushButton>
#include <QGroupBox>

TgSettingsDialog::TgSettingsDialog(const TgAppleConfig& config, QWidget* parent)
    : QDialog(parent), m_config(config)
{
    setModal(true);
    setWindowTitle(tr("游戏设置"));
    resize(320, 280);
    setupUI();
}

void TgSettingsDialog::setupUI()
{
    auto* mainLayout = new QVBoxLayout(this);
    auto* formLayout = new QVBoxLayout();

    m_sliderLevel = new QSlider(Qt::Horizontal, this);
    m_sliderLevel->setRange(0, 9); m_sliderLevel->setValue(m_config.level);
    m_lblLevel = new QLabel(tr("等级: %1").arg(m_config.level), this);
    connect(m_sliderLevel, &QSlider::valueChanged, [this](int v) {
        m_lblLevel->setText(tr("等级: %1").arg(v)); m_config.level = v; });
    formLayout->addWidget(m_lblLevel); formLayout->addWidget(m_sliderLevel);

    m_sliderFails = new QSlider(Qt::Horizontal, this);
    m_sliderFails->setRange(5, 50); m_sliderFails->setValue(m_config.maxFails);
    m_lblFails = new QLabel(tr("失败上限: %1").arg(m_config.maxFails), this);
    connect(m_sliderFails, &QSlider::valueChanged, [this](int v) {
        m_lblFails->setText(tr("失败上限: %1").arg(v)); m_config.maxFails = v; });
    formLayout->addWidget(m_lblFails); formLayout->addWidget(m_sliderFails);

    m_sliderTarget = new QSlider(Qt::Horizontal, this);
    m_sliderTarget->setRange(100, 900); m_sliderTarget->setValue(m_config.target);
    m_lblTarget = new QLabel(tr("胜利目标: %1").arg(m_config.target), this);
    connect(m_sliderTarget, &QSlider::valueChanged, [this](int v) {
        m_lblTarget->setText(tr("胜利目标: %1").arg(v)); m_config.target = v; });
    formLayout->addWidget(m_lblTarget); formLayout->addWidget(m_sliderTarget);

    m_checkSound = new QCheckBox(tr("开启音效"), this);
    m_checkSound->setChecked(m_config.soundEnabled);
    connect(m_checkSound, &QCheckBox::toggled, [this](bool checked) { m_config.soundEnabled = checked; });
    formLayout->addWidget(m_checkSound);

    auto* btnLayout = new QHBoxLayout();
    m_btnOk = new QPushButton(tr("确定"), this);
    m_btnCancel = new QPushButton(tr("取消"), this);
    m_btnDefault = new QPushButton(tr("恢复默认"), this);
    btnLayout->addWidget(m_btnDefault); btnLayout->addStretch();
    btnLayout->addWidget(m_btnOk); btnLayout->addWidget(m_btnCancel);

    mainLayout->addLayout(formLayout); mainLayout->addLayout(btnLayout);

    connect(m_btnOk, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_btnDefault, &QPushButton::clicked, [this]() {
        m_sliderLevel->setValue(3); m_sliderFails->setValue(10);
        m_sliderTarget->setValue(100); m_checkSound->setChecked(true); });
}

TgAppleConfig TgSettingsDialog::getConfig() const { return m_config; }