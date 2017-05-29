#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QPointer>
#include <QVariant>

class QSettings;

// Settings
class Settings : public QObject
{
    Q_OBJECT

public:
    explicit Settings(QObject *parent = Q_NULLPTR);

    int updateInterval() const { return _updateInterval; }
    void setUpdateInterval(const int updateInterval);
    Q_SIGNAL void updateIntervalChanged(const int updateInterval);

    int displayInterval() const { return _displayInterval; }
    void setDisplayInterval(const int displayInterval);
    Q_SIGNAL void displayIntervalChanged(const int displayInterval);

    bool showMessages() const { return _showMessages; }
    void setShowMessages(const int showMessages);
    Q_SIGNAL void showMessagesChanged(const bool showMessages);

    bool playSound() const { return _playSound; }
    void setPlaySound(const int playSound);
    Q_SIGNAL void playSoundChanged(const bool playSound);

    QVariant value(const QString &key, const QVariant &defaultValue = QVariant());
    void setValue(const QString &key, const QVariant &value);

    static const int minUpdateInterval;
    static const int minDisplayInterval;

private:
    static const QString _keyUpdateInterval;
    static const QString _keyDisplayInterval;
    static const QString _keyShowMessages;
    static const QString _keyPlaySound;

    QPointer<QSettings> _settings;
    int _updateInterval = 2 * minUpdateInterval;
    int _displayInterval = 2 * minDisplayInterval;
    bool _showMessages = true;
    bool _playSound = true;
};

#endif // SETTINGS_H
