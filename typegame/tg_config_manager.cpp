#include "tg_config_manager.h"

TgConfigManager::TgConfigManager(const QString& filePath)
    : m_settings(filePath, QSettings::IniFormat)
{
}

void TgConfigManager::loadAppleConfig(TgAppleConfig& config)
{
    m_settings.beginGroup("Apple");
    config.level = qBound(0, m_settings.value("Level", 3).toInt(), 9);
    config.target = qBound(100, m_settings.value("Target", 100).toInt(), 900);
    config.maxFails = qBound(5, m_settings.value("MaxFails", 10).toInt(), 50);
    config.maxOnScreen = qBound(1, m_settings.value("MaxOnScreen", 5).toInt(), 5);
    config.soundEnabled = m_settings.value("SoundEnabled", true).toBool();
    m_settings.endGroup();
}

void TgConfigManager::saveAppleConfig(const TgAppleConfig& config)
{
    m_settings.beginGroup("Apple");
    m_settings.setValue("Level", config.level);
    m_settings.setValue("Target", config.target);
    m_settings.setValue("MaxFails", config.maxFails);
    m_settings.setValue("MaxOnScreen", config.maxOnScreen);
    m_settings.setValue("SoundEnabled", config.soundEnabled);
    m_settings.endGroup();
    m_settings.sync();
}