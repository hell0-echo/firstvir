// tg_confirm_dialog.h
#ifndef _TG_CONFIRM_DIALOG_H_
#define _TG_CONFIRM_DIALOG_H_
#include <QDialog>
class QPushButton;
class TgConfirmDialog : public QDialog {
    Q_OBJECT
public:
    explicit TgConfirmDialog(const QString& message, QWidget* parent = nullptr);
    ~TgConfirmDialog() override = default;
    TgConfirmDialog(const TgConfirmDialog&) = delete;
    TgConfirmDialog& operator=(const TgConfirmDialog&) = delete;
private:
    QPushButton* m_btnYes;
    QPushButton* m_btnNo;
};
#endif

