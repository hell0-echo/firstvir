// tg_settings_dialog.h
#ifndef _TG_SETTINGS_DIALOG_H_
#define _TG_SETTINGS_DIALOG_H_
#include <QDialog>
#include "tg_config_manager.h"

class QSlider;
class QCheckBox;
class QLabel;
class QPushButton;

class TgSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TgSettingsDialog(const TgAppleConfig& config, QWidget* parent = nullptr);
    ~TgSettingsDialog() override = default;

    TgSettingsDialog(const TgSettingsDialog&) = delete;
    TgSettingsDialog& operator=(const TgSettingsDialog&) = delete;

    TgAppleConfig getConfig() const;

signals:
    void applyRequested();

private:
    void setupUI();
    QSlider* m_sliderLevel;
    QSlider* m_sliderFails;
    QSlider* m_sliderTarget;
    QCheckBox* m_checkSound;
    QLabel* m_lblLevel;
    QLabel* m_lblFails;
    QLabel* m_lblTarget;
    QPushButton* m_btnOk;
    QPushButton* m_btnCancel;
    QPushButton* m_btnDefault;
    TgAppleConfig m_config;
};
#endif

