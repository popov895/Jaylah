#ifndef LMSENSORS_H
#define LMSENSORS_H

#include <QColor>
#include <QObject>

#include <sensors/sensors.h>

// SensorData
struct SensorData
{
    const sensors_chip_name *chip;
    const int subfeature;
    const QString name;
    const QString id;
};

// SensorProperties
struct SensorProperties
{
    bool enabled;
    QColor color;
    bool alertsEnabled;
    int alertsThreshold;
};

// Sensor
class Sensor : public QObject
{
    Q_OBJECT

    friend class Chip;

private:
    Sensor(const SensorData &data, QObject *parent);
    virtual ~Sensor() {}

public:
    const QString& id() const { return _data.id; }
    const QString& name() const { return _data.name; }

    double value() const { return _value; }
    Q_SIGNAL void valueChanged(const double value);

    const SensorProperties& properties() const { return _properties; }
    void setProperties(const SensorProperties &properties);
    Q_SIGNAL void colorChanged(const QColor &color);
    Q_SIGNAL void enabledChanged(const bool enabled);
    Q_SIGNAL void alertsEnabledChanged(const bool alertsEnabled);
    Q_SIGNAL void alertsThresholdChanged(const int alertsThreshold);

    bool alert() const { return _alert; }
    Q_SIGNAL void alertChanged(const bool alert);

    void update();

private:
    void checkAlert();

    const SensorData _data;
    SensorProperties _properties = { true, Qt::darkGreen, false, false };
    double _value = 0;
    bool _alert = false;
};

typedef QList<Sensor*> SensorList;

// ChipData
struct ChipData
{
    const sensors_chip_name *chip;
    const QString name;
    const QString id;
};

// Chip
class Chip : public QObject
{
    Q_OBJECT

    friend class LmSensors;

private:
    Chip(const ChipData &data, QObject *parent);
    virtual ~Chip() {}

public:
    const QString& id() const { return _data.id; }
    const QString& name() const { return _data.name; }
    const SensorList& sensors() const { return _sensors; }

private:
    const ChipData _data;
    SensorList _sensors;
};

typedef QList<Chip*> ChipList;

// LmSensors
class LmSensors : public QObject
{
    Q_OBJECT

private:
    LmSensors();
    virtual ~LmSensors() { sensors_cleanup(); }

public:
    static const ChipList& chips();

private:
    ChipList _chips;
};

#endif // LMSENSORS_H
