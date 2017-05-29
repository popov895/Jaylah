#ifndef ALERTSWATCHER_H
#define ALERTSWATCHER_H

#include <QObject>

class Sensor;

// AlertsWatcher
class AlertsWatcher : public QObject
{
    Q_OBJECT

public:
    explicit AlertsWatcher(QObject *parent = Q_NULLPTR);

    bool alert() const { return !_sensors.isEmpty(); }
    Q_SIGNAL void alertChanged(const bool alert);

    Sensor* sensor() const { return (_sensors.isEmpty() ? Q_NULLPTR : _sensors.last()); }
    Q_SIGNAL void sensorChanged();

private:
    QList<Sensor*> _sensors;
};

#endif // ALERTSWATCHER_H
