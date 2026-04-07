/* ------------------------------------------------------------------
//  文件名    :   tg_apple_bottom_toolbar.h
//  创建者    :   dev@project.cn
//  创建时间  :   2024-05-20
//  功能描述  :   拯救苹果底部操作栏（左下角固定，状态联动）
// ----------------------------------------------------------------*/
#ifndef _TG_APPLE_BOTTOM_TOOLBAR_H_
#define _TG_APPLE_BOTTOM_TOOLBAR_H_

#include <QWidget>
#include "tg_common_types.h"
#include "tg_custom_button.h"

class TgAppleBottomToolbar : public QWidget
{
    Q_OBJECT
public:
    explicit TgAppleBottomToolbar(QWidget* parent = nullptr);
    ~TgAppleBottomToolbar() override = default;

    TgAppleBottomToolbar(const TgAppleBottomToolbar&) = delete;
    TgAppleBottomToolbar& operator=(const TgAppleBottomToolbar&) = delete;

public slots:
    void updateButtonStates(TgGameState state);

signals:
    void pauseClicked();
    void startClicked();
    void endClicked();
    void settingsClicked();
    void exitClicked();

private:
    void setupUI();

    TgCustomButton* m_btnPause;
    TgCustomButton* m_btnStart;
    TgCustomButton* m_btnEnd;
    TgCustomButton* m_btnSettings;
    TgCustomButton* m_btnExit;
};

#endif // _TG_APPLE_BOTTOM_TOOLBAR_H_

