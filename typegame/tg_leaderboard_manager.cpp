// tg_leaderboard_manager.cpp
#include "tg_leaderboard_manager.h"
#include <algorithm>

TgLeaderboardManager::TgLeaderboardManager(const QString& filePath)
    : m_settings(filePath, QSettings::IniFormat)
{
}

void TgLeaderboardManager::load()
{
    m_entries.clear();
    int count = m_settings.beginReadArray("Entries");
    for (int i = 0; i < count; ++i) {
        m_settings.setArrayIndex(i);
        TgLeaderboardEntry entry;
        entry.name = m_settings.value("Name").toString();
        entry.score = m_settings.value("Score").toInt();
        m_entries.append(entry);
    }
    m_settings.endArray();
    sortEntries();
}

void TgLeaderboardManager::save()
{
    m_settings.beginWriteArray("Entries");
    for (int i = 0; i < m_entries.size(); ++i) {
        m_settings.setArrayIndex(i);
        m_settings.setValue("Name", m_entries[i].name);
        m_settings.setValue("Score", m_entries[i].score);
    }
    m_settings.endArray();
    m_settings.sync();
}

void TgLeaderboardManager::addEntry(const QString& name, int score)
{
    // 检查同名更新最高分
    for (auto& entry : m_entries) {
        if (entry.name == name) {
            if (score > entry.score) entry.score = score;
            sortEntries();
            return;
        }
    }
    // 新增
    m_entries.append({ name, score });
    sortEntries();
}

void TgLeaderboardManager::sortEntries()
{
    std::sort(m_entries.begin(), m_entries.end(), [](const TgLeaderboardEntry& a, const TgLeaderboardEntry& b) {
        return a.score > b.score;
        });
    if (m_entries.size() > 10) {
        m_entries.resize(10);
    }
}

QVector<TgLeaderboardEntry> TgLeaderboardManager::getTopEntries(int count) const
{
    return m_entries.mid(0, qMin(count, m_entries.size()));
}