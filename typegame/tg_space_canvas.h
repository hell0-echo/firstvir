#ifndef _TG_SPACE_CANVAS_H_
#define _TG_SPACE_CANVAS_H_

#include <QWidget>
#include <QPixmap>

// 前向声明
class TgSpaceGameLogic;

class TgSpaceCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit TgSpaceCanvas(TgSpaceGameLogic* logic, QWidget* parent = nullptr);
    ~TgSpaceCanvas() override = default;
    TgSpaceCanvas(const TgSpaceCanvas&) = delete;
    TgSpaceCanvas& operator=(const TgSpaceCanvas&) = delete;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    TgSpaceGameLogic* m_logic = nullptr;
    QPixmap m_backBuffer;
};

#endif // _TG_SPACE_CANVAS_H_