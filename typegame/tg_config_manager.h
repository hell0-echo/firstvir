/* ------------------------------------------------------------------
//  文件名    :   tg_config_manager.h
//  创建者    :   dev@project.cn
//  创建时间  :   2024-05-20
//  功能描述  :   INI 配置持久化管理（苹果参数、音效开关）
// ----------------------------------------------------------------*/
#ifndef _TG_CONFIG_MANAGER_H_
#define _TG_CONFIG_MANAGER_H_

#include <QSettings>
#include <QString>

struct TgAppleConfig
{
    int level = 3;
    int target = 100;
    int maxFails = 10;
    int maxOnScreen = 5;
    bool soundEnabled = true;
};

class TgConfigManager
{
public:
    explicit TgConfigManager(const QString& filePath);
    ~TgConfigManager() = default;

    TgConfigManager(const TgConfigManager&) = delete;
    TgConfigManager& operator=(const TgConfigManager&) = delete;

    void loadAppleConfig(TgAppleConfig& config);
    void saveAppleConfig(const TgAppleConfig& config);

private:
    QSettings m_settings;
};

#endif // _TG_CONFIG_MANAGER_H_