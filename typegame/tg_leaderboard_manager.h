/* ------------------------------------------------------------------
//  文件名    :   tg_leaderboard_manager.h
//  创建者    :   dev@project.cn
//  创建时间  :   2024-05-20
//  功能描述  :   排行榜管理：INI 读写、Top10 降序、同名更新最高分
// ----------------------------------------------------------------*/
#ifndef _TG_LEADERBOARD_MANAGER_H_
#define _TG_LEADERBOARD_MANAGER_H_

#include <QVector>
#include <QString>
#include <QSettings>

struct TgLeaderboardEntry
{
    QString name;
    int score;
};

class TgLeaderboardManager
{
public:
    explicit TgLeaderboardManager(const QString& filePath);
    ~TgLeaderboardManager() = default;

    TgLeaderboardManager(const TgLeaderboardManager&) = delete;
    TgLeaderboardManager& operator=(const TgLeaderboardManager&) = delete;

    void load();
    void save(); // 移除了 const
    void addEntry(const QString& name, int score);
    QVector<TgLeaderboardEntry> getTopEntries(int count = 10) const;

private:
    void sortEntries();
    QVector<TgLeaderboardEntry> m_entries;
    QSettings m_settings;
};

#endif // _TG_LEADERBOARD_MANAGER_H_

