#ifndef _TG_SPACE_CANVAS_H_
#define _TG_SPACE_CANVAS_H_
#include <QWidget>
#include <QPixmap>
#include "tg_thread_sync.h"

class TgSpaceCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit TgSpaceCanvas(TgThreadSync& sync, QWidget* parent = nullptr);
    ~TgSpaceCanvas() override = default;
private slots:
    void scheduleUpdate();
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    TgThreadSync& m_sync;
    QPixmap m_backBuffer;
};
#endif