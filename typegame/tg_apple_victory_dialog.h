/* ------------------------------------------------------------------
//  文件名    :   tg_apple_victory_dialog.h
//  创建者    :   dev@project.cn
//  创建时间  :   2024-05-20
//  功能描述  :   拯救苹果胜利弹窗（继续/下一关/结束）
// ----------------------------------------------------------------*/
#ifndef _TG_APPLE_VICTORY_DIALOG_H_
#define _TG_APPLE_VICTORY_DIALOG_H_

#include <QDialog>

class QPushButton;
class QLabel;

enum TgVictoryResult
{
    TgVictoryContinue = 0,
    TgVictoryNextLevel = 1,
    TgVictoryEnd = 2
};

class TgAppleVictoryDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TgAppleVictoryDialog(QWidget* parent = nullptr);
    ~TgAppleVictoryDialog() override = default;

    TgAppleVictoryDialog(const TgAppleVictoryDialog&) = delete;
    TgAppleVictoryDialog& operator=(const TgAppleVictoryDialog&) = delete;

private:
    void setupUI();

    QLabel* m_lblMessage;
    QPushButton* m_btnContinue;
    QPushButton* m_btnNextLevel;
    QPushButton* m_btnEnd;
};

#endif // _TG_APPLE_VICTORY_DIALOG_H_