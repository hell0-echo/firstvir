#include "tg_custom_button.h"
#include <QPainter>
#include <QMouseEvent>

TgCustomButton::TgCustomButton(const QString& text, QWidget* parent)
    : QPushButton(text, parent)
    , m_currentState(TgButtonState::Normal)
{
    setFixedSize(150, 50);
    setFocusPolicy(Qt::NoFocus);
}

void TgCustomButton::setButtonStates(const QPixmap& normal, const QPixmap& hover,
    const QPixmap& pressed, const QPixmap& disabled)
{
    m_pixNormal = normal;
    m_pixHover = hover;
    m_pixPressed = pressed;
    m_pixDisabled = disabled;
    update();
}

void TgCustomButton::enterEvent(QEvent* event)
{
    if (isEnabled())
    {
        m_currentState = TgButtonState::Hover;
        update();
    }
    QPushButton::enterEvent(event);
}

void TgCustomButton::leaveEvent(QEvent* event)
{
    if (isEnabled())
    {
        m_currentState = TgButtonState::Normal;
        update();
    }
    QPushButton::leaveEvent(event);
}

void TgCustomButton::mousePressEvent(QMouseEvent* event)
{
    if (isEnabled())
    {
        m_currentState = TgButtonState::Pressed;
        update();
    }
    QPushButton::mousePressEvent(event);
}

void TgCustomButton::mouseReleaseEvent(QMouseEvent* event)
{
    if (isEnabled())
    {
        m_currentState = TgButtonState::Hover;
        update();
    }
    QPushButton::mouseReleaseEvent(event);
}

void TgCustomButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPixmap* pix = nullptr;
    switch (m_currentState)
    {
    case TgButtonState::Normal:
        pix = &m_pixNormal;
        break;
    case TgButtonState::Hover:
        pix = &m_pixHover;
        break;
    case TgButtonState::Pressed:
        pix = &m_pixPressed;
        break;
    case TgButtonState::Disabled:
        pix = &m_pixDisabled;
        break;
    }

    if (pix && !pix->isNull())
    {
        painter.drawPixmap(rect(), *pix);
    }
    else
    {
        // Fallback: draw colored rectangle
        QColor color;
        switch (m_currentState)
        {
        case TgButtonState::Normal:
            color = QColor(70, 130, 180);
            break;
        case TgButtonState::Hover:
            color = QColor(100, 149, 237);
            break;
        case TgButtonState::Pressed:
            color = QColor(65, 105, 225);
            break;
        case TgButtonState::Disabled:
            color = QColor(128, 128, 128);
            break;
        }
        painter.fillRect(rect(), color);
    }

    painter.setPen(Qt::white);
    painter.setFont(font());
    painter.drawText(rect(), Qt::AlignCenter, text());
}

void TgCustomButton::updateState()
{
    if (!isEnabled())
    {
        m_currentState = TgButtonState::Disabled;
    }
    else
    {
        m_currentState = TgButtonState::Normal;
    }
    update();
}