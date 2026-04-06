// tg_game_loop.cpp
#include "tg_game_loop.h"

TgGameLoop::TgGameLoop(QObject* parent)
    : QObject(parent)
{
    m_timer.setInterval(16);
    m_timer.setTimerType(Qt::PreciseTimer);
    connect(&m_timer, &QTimer::timeout, this, &TgGameLoop::tick);
    moveToThread(&m_thread);
    m_thread.start();
}

TgGameLoop::~TgGameLoop()
{
    m_timer.stop();
    m_thread.quit();
    m_thread.wait();
}

void TgGameLoop::start() { m_timer.start(); }
void TgGameLoop::stop() { m_timer.stop(); }