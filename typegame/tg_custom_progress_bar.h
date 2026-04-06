/* ------------------------------------------------------------------
//  文件名    :   tg_custom_progress_bar.h
//  创建者    :   dev@project.cn
//  创建时间  :   2024-05-20
//  功能描述  :   自定义进度条：18HP 分段渲染
// ----------------------------------------------------------------*/
#ifndef _TG_CUSTOM_PROGRESS_BAR_H_
#define _TG_CUSTOM_PROGRESS_BAR_H_

#include <QWidget>

class TgCustomProgressBar : public QWidget
{
    Q_OBJECT
public:
    explicit TgCustomProgressBar(QWidget* parent = nullptr);
    ~TgCustomProgressBar() override = default;

    TgCustomProgressBar(const TgCustomProgressBar&) = delete;
    TgCustomProgressBar& operator=(const TgCustomProgressBar&) = delete;

    void setCurrentValue(int value);
    void setMaxValue(int max);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    int m_currentValue = 18;
    int m_maxValue = 18;
    const int m_segments = 18; // 固定 18 段
};

#endif // _TG_CUSTOM_PROGRESS_BAR_H_

