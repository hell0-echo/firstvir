/* ------------------------------------------------------------------
//  文件名    :   tg_exit_dialog.h
//  创建者    :   dev@project.cn
//  创建时间  :   2024-05-20
//  功能描述  :   退出确认弹窗（是/否）
// ----------------------------------------------------------------*/
#ifndef _TG_EXIT_DIALOG_H_
#define _TG_EXIT_DIALOG_H_

#include <QDialog>

class QPushButton;
class QLabel;

class TgExitDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TgExitDialog(QWidget* parent = nullptr);
    ~TgExitDialog() override = default;

    TgExitDialog(const TgExitDialog&) = delete;
    TgExitDialog& operator=(const TgExitDialog&) = delete;

private:
    void setupUI();

    QLabel* m_lblMessage;
    QPushButton* m_btnYes;
    QPushButton* m_btnNo;
};

#endif // _TG_EXIT_DIALOG_H_