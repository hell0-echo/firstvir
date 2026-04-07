#ifndef _TG_GAME_CANVAS_H_
#define _TG_GAME_CANVAS_H_

#include <QWidget>
#include <QPixmap>

// 前向声明，避免头文件循环依赖
class TgAppleLogicOrchestrator;

class TgGameCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit TgGameCanvas(TgAppleLogicOrchestrator* logic, QWidget* parent = nullptr);
    ~TgGameCanvas() override = default;
    TgGameCanvas(const TgGameCanvas&) = delete;
    TgGameCanvas& operator=(const TgGameCanvas&) = delete;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    TgAppleLogicOrchestrator* m_logic = nullptr;
    QPixmap m_backBuffer;
};

#endif // _TG_GAME_CANVAS_H_