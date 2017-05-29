#include "AlertsWatcher.h"

#include "LmSensors.h"

// AlertsWatcher

AlertsWatcher::AlertsWatcher(QObject *parent)
    : QObject(parent)
{
    for (auto chip : LmSensors::chips()) {
        for (auto sensor : chip->sensors()) {
            connect(sensor, &Sensor::alertChanged, [this, sensor] (const bool alert) {
                if (alert) {
                    Q_ASSERT(!_sensors.contains(sensor));
                    _sensors << sensor;
                    if (_sensors.size() > 1)
                        Q_EMIT sensorChanged();
                    else
                        Q_EMIT alertChanged(true);
                } else {
                    if (!_sensors.removeOne(sensor))
                        Q_ASSERT(false);
                    if (_sensors.isEmpty())
                        Q_EMIT alertChanged(false);
                }
            });

            if (sensor->alert())
                _sensors << sensor;
        }
    }
}
