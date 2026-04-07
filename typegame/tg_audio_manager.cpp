#include "tg_audio_manager.h"

TgAudioManager::TgAudioManager(QObject* parent)
    : QObject(parent), m_enabled(true)
{
    m_sfxHit = new QSoundEffect(this);
    m_sfxHit->setSource(QUrl::fromLocalFile(":/audio/hit.wav"));

    m_sfxFail = new QSoundEffect(this);
    m_sfxFail->setSource(QUrl::fromLocalFile(":/audio/fail.wav"));

    m_sfxClick = new QSoundEffect(this);
    m_sfxClick->setSource(QUrl::fromLocalFile(":/audio/click.wav"));

    m_bgmPlayer = new QMediaPlayer(this);
    m_bgmPlayer->setMedia(QUrl::fromLocalFile(":/audio/bgm.mp3"));

    // Qt5 循环播放兼容写法
    connect(m_bgmPlayer, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia && m_enabled) {
            m_bgmPlayer->play();
        }
        });
}

void TgAudioManager::setSoundEnabled(bool enabled)
{
    m_enabled = enabled;
    if (!m_enabled) {
        stopBGM();
    }
    else if (m_bgmPlayer->state() != QMediaPlayer::PlayingState) {
        playBGM();
    }
}

void TgAudioManager::playSFX(TgSFXType type)
{
    if (!m_enabled) return;
    switch (type)
    {
    case TgSFXType::Hit:
        if (m_sfxHit->isLoaded()) m_sfxHit->play();
        break;
    case TgSFXType::Fail:
        if (m_sfxFail->isLoaded()) m_sfxFail->play();
        break;
    case TgSFXType::ButtonClick:
        if (m_sfxClick->isLoaded()) m_sfxClick->play();
        break;
    default: break;
    }
}

void TgAudioManager::playBGM()
{
    if (m_enabled && m_bgmPlayer->state() != QMediaPlayer::PlayingState)
        m_bgmPlayer->play();
}

void TgAudioManager::stopBGM()
{
    m_bgmPlayer->stop();
}