/* ------------------------------------------------------------------
//  文件名    :   tg_custom_button.h
//  创建者    :   dev@project.cn
//  创建时间  :   2024-05-20
//  功能描述  :   自定义四态按钮（正常/悬停/按下/禁用）
// ----------------------------------------------------------------*/
#ifndef _TG_CUSTOM_BUTTON_H_
#define _TG_CUSTOM_BUTTON_H_

#include <QPushButton>
#include <QPixmap>

enum class TgButtonState
{
    Normal,
    Hover,
    Pressed,
    Disabled
};

class TgCustomButton : public QPushButton
{
    Q_OBJECT

public:
    explicit TgCustomButton(const QString& text, QWidget* parent = nullptr);
    ~TgCustomButton() override = default;

    TgCustomButton(const TgCustomButton&) = delete;
    TgCustomButton& operator=(const TgCustomButton&) = delete;

    void setButtonStates(const QPixmap& normal, const QPixmap& hover,
        const QPixmap& pressed, const QPixmap& disabled);

protected:
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    void updateState();

    QPixmap m_pixNormal;
    QPixmap m_pixHover;
    QPixmap m_pixPressed;
    QPixmap m_pixDisabled;
    TgButtonState m_currentState;
};

#endif // _TG_CUSTOM_BUTTON_H_