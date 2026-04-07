/* ------------------------------------------------------------------
//  文件名    :   tg_main_menu.h
//  创建者    :   dev@project.cn
//  创建时间  :   2024-05-20
//  功能描述  :   游戏主菜单界面（选择拯救苹果/太空大战）
// ----------------------------------------------------------------*/
#ifndef _TG_MAIN_MENU_H_
#define _TG_MAIN_MENU_H_

#include <QWidget>
#include "tg_common_types.h"

class TgCustomButton;

class TgMainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit TgMainMenu(QWidget* parent = nullptr);
    ~TgMainMenu() override = default;

    TgMainMenu(const TgMainMenu&) = delete;
    TgMainMenu& operator=(const TgMainMenu&) = delete;

signals:
    void gameModeSelected(TgGameMode mode);
    void exitRequested();
    void settingsRequested();

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onAppleGameClicked();
    void onSpaceGameClicked();
    void onSettingsClicked();
    void onExitClicked();

private:
    void setupUI();

    TgCustomButton* m_btnAppleGame;
    TgCustomButton* m_btnSpaceGame;
    TgCustomButton* m_btnSettings;
    TgCustomButton* m_btnExit;
    QPixmap m_background;
};

#endif // _TG_MAIN_MENU_H_