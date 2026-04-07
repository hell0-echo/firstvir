#ifndef _TG_GAME_LOOP_H_
#define _TG_GAME_LOOP_H_

#include <QTimer>
#include <QThread>

class TgGameLoop : public QObject
{
    Q_OBJECT
public:
    explicit TgGameLoop(QObject* parent = nullptr);
    ~TgGameLoop() override;

    void start();
    void stop();

signals:
    void tick();

private:
    QThread m_thread;
    QTimer m_timer;
};

#endif // _TG_GAME_LOOP_H_

