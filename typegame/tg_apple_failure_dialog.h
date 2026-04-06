/* ------------------------------------------------------------------
//  文件名    :   tg_apple_failure_dialog.h
//  创建者    :   dev@project.cn
//  创建时间  :   2024-05-20
//  功能描述  :   拯救苹果失败弹窗（继续/结束）
// ----------------------------------------------------------------*/
#ifndef _TG_APPLE_FAILURE_DIALOG_H_
#define _TG_APPLE_FAILURE_DIALOG_H_

#include <QDialog>

class QPushButton;
class QLabel;

enum TgFailureResult
{
    TgFailureContinue = 0,
    TgFailureEnd = 1
};

class TgAppleFailureDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TgAppleFailureDialog(QWidget* parent = nullptr);
    ~TgAppleFailureDialog() override = default;

    TgAppleFailureDialog(const TgAppleFailureDialog&) = delete;
    TgAppleFailureDialog& operator=(const TgAppleFailureDialog&) = delete;

private:
    void setupUI();

    QLabel* m_lblMessage;
    QPushButton* m_btnContinue;
    QPushButton* m_btnEnd;
};

#endif // _TG_APPLE_FAILURE_DIALOG_H_