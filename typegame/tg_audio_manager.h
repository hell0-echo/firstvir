/* ------------------------------------------------------------------
//  文件名    :   tg_audio_manager.h
//  创建者    :   dev@project.cn
//  创建时间  :   2024-05-20
//  功能描述  :   音频路由管理（BGM 循环、SFX 触发、开关控制）
// ----------------------------------------------------------------*/
#ifndef _TG_AUDIO_MANAGER_H_
#define _TG_AUDIO_MANAGER_H_

#include <QObject>
#include <QSoundEffect>
#include <QMediaPlayer>

enum class TgSFXType
{
    Hit,
    Fail,
    ButtonClick,
    BGM
};

class TgAudioManager : public QObject
{
    Q_OBJECT
public:
    explicit TgAudioManager(QObject* parent = nullptr);
    ~TgAudioManager() override = default;

    TgAudioManager(const TgAudioManager&) = delete;
    TgAudioManager& operator=(const TgAudioManager&) = delete;

    void setSoundEnabled(bool enabled);
    void playSFX(TgSFXType type);
    void playBGM();
    void stopBGM();

private:
    bool m_enabled;
    QSoundEffect* m_sfxHit;
    QSoundEffect* m_sfxFail;
    QSoundEffect* m_sfxClick;
    QMediaPlayer* m_bgmPlayer;
};

#endif // _TG_AUDIO_MANAGER_H_

