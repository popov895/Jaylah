#include "Settings.h"

#include <QSettings>

// Settings

const int Settings::minUpdateInterval = 1;
const int Settings::minDisplayInterval = 30;

const QString Settings::_keyUpdateInterval = "updateInterval";
const QString Settings::_keyDisplayInterval = "displayInterval";
const QString Settings::_keyShowMessages = "showMessages";
const QString Settings::_keyPlaySound = "playSound";

Settings::Settings(QObject *parent)
    : QObject(parent)
    , _settings(new QSettings(this))
{
    _updateInterval = qMax(minUpdateInterval, value(_keyUpdateInterval, _updateInterval).toInt());
    _displayInterval = qMax(minDisplayInterval, value(_keyDisplayInterval, _displayInterval).toInt());
    _showMessages = value(_keyShowMessages, _showMessages).toBool();
    _playSound = value(_keyPlaySound, _playSound).toBool();
}

void Settings::setUpdateInterval(const int updateInterval)
{
    Q_ASSERT(updateInterval >= minUpdateInterval);

    if (updateInterval != _updateInterval) {
        Q_EMIT updateIntervalChanged(_updateInterval = updateInterval);
        setValue(_keyUpdateInterval, _updateInterval);
    }
}

void Settings::setDisplayInterval(const int displayInterval)
{
    Q_ASSERT(displayInterval >= minDisplayInterval);

    if (displayInterval != _displayInterval) {
        Q_EMIT displayIntervalChanged(_displayInterval = displayInterval);
        setValue(_keyDisplayInterval, _displayInterval);
    }
}

void Settings::setShowMessages(const int showMessages)
{
    if (showMessages != _showMessages) {
        Q_EMIT showMessagesChanged(_showMessages = showMessages);
        setValue(_keyShowMessages, _showMessages);
    }
}

void Settings::setPlaySound(const int playSound)
{
    if (playSound != _playSound) {
        Q_EMIT playSoundChanged(_playSound = playSound);
        setValue(_keyPlaySound, _playSound);
    }
}

QVariant Settings::value(const QString &key, const QVariant &defaultValue)
{
    return _settings->value(key, defaultValue);
}

void Settings::setValue(const QString &key, const QVariant &value)
{
    _settings->setValue(key, value);
}
