// tg_game_canvas.h
#ifndef _TG_GAME_CANVAS_H_
#define _TG_GAME_CANVAS_H_

#include <QWidget>
#include <QPixmap>
#include "tg_thread_sync.h"

class TgGameCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit TgGameCanvas(TgThreadSync& sync, QWidget* parent = nullptr);
    ~TgGameCanvas() override = default;

    TgGameCanvas(const TgGameCanvas&) = delete;
    TgGameCanvas& operator=(const TgGameCanvas&) = delete;

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void scheduleUpdate();

private:
    TgThreadSync& m_sync;
    QPixmap m_backBuffer;
};

#endif // _TG_GAME_CANVAS_H_

