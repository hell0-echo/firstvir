/* ------------------------------------------------------------------
//  文件名    :   tg_reward_word_entity.h
//  创建者    :   dev@project.cn
//  创建时间  :   2024-05-20
//  功能描述  :   奖励单词实体：横向飞过，完整输入后恢复满血
// ----------------------------------------------------------------*/
#ifndef _TG_REWARD_WORD_ENTITY_H_
#define _TG_REWARD_WORD_ENTITY_H_

#include "tg_entity_base.h"
#include <QString>

class TgRewardWordEntity : public TgEntityBase
{
public:
    TgRewardWordEntity();
    ~TgRewardWordEntity() override = default;

    TgRewardWordEntity(const TgRewardWordEntity&) = delete;
    TgRewardWordEntity& operator=(const TgRewardWordEntity&) = delete;

    void update(float dt) override;

    // 输入匹配逻辑：检查字符是否匹配单词的下一个字符
    bool matchChar(QChar ch);

    // 检查单词是否已完全输入
    bool isCompleted() const;

    // 重置并设置新单词，激活实体
    void reset(const QString& word);

    // 渲染辅助接口
    QString getWord() const { return m_word; }
    int getTypedLength() const { return m_typedLength; }

private:
    QString m_word;       // 目标单词
    int m_typedLength;    // 当前已正确输入的字符数
};

#endif // _TG_REWARD_WORD_ENTITY_H_