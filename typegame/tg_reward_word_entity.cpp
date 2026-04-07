/* ------------------------------------------------------------------
//  文件名    :   tg_reward_word_entity.cpp
//  创建者    :   dev@project.cn
//  创建时间  :   2024-05-20
//  功能描述  :   奖励单词实体实现
// ----------------------------------------------------------------*/
#include "tg_reward_word_entity.h"

TgRewardWordEntity::TgRewardWordEntity()
    : m_typedLength(0)
{
}

void TgRewardWordEntity::update(float dt)
{
    if (!m_isActive) return;

    // 横向移动 (通常奖励单词是水平飞过)
    m_pos.rx() += m_speed * dt;

    // 边界检查：飞出屏幕右侧后失效
    // 假设游戏逻辑宽度上限约为 1200 (适配高分屏)
    if (m_pos.x() > 1200.0f)
    {
        m_isActive = false;
    }
}

bool TgRewardWordEntity::matchChar(QChar ch)
{
    if (!m_isActive) return false;
    if (m_typedLength >= m_word.length()) return false;

    // 不区分大小写匹配
    if (ch.toUpper() == m_word[m_typedLength].toUpper())
    {
        m_typedLength++;
        return true;
    }
    return false;
}

bool TgRewardWordEntity::isCompleted() const
{
    return m_typedLength >= m_word.length();
}

void TgRewardWordEntity::reset(const QString& word)
{
    m_word = word;
    m_typedLength = 0;
    m_isActive = true;
}