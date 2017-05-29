#include "LmSensors.h"

#include <QScopedPointer>

// Sensor

Sensor::Sensor(const SensorData &data, QObject *parent)
    : QObject(parent)
    , _data(data)
{}

void Sensor::setProperties(const SensorProperties &properties)
{
    if (properties.color != _properties.color)
        Q_EMIT colorChanged(_properties.color = properties.color);

    auto needCheckAlert = false;

    if (properties.enabled != _properties.enabled) {
        Q_EMIT enabledChanged(_properties.enabled = properties.enabled);
        needCheckAlert = true;
    }

    if (properties.alertsEnabled != _properties.alertsEnabled) {
        Q_EMIT alertsEnabledChanged(_properties.alertsEnabled = properties.alertsEnabled);
        needCheckAlert = true;
    }

    if (properties.alertsThreshold != _properties.alertsThreshold) {
        Q_EMIT alertsThresholdChanged(_properties.alertsThreshold = properties.alertsThreshold);
        needCheckAlert = true;
    }

    if (needCheckAlert)
        checkAlert();
}

void Sensor::update()
{
    if (!_properties.enabled)
        return;

    Q_CHECK_PTR(_data.chip);

    auto value = 0.;
    sensors_get_value(_data.chip, _data.subfeature, &value);

    if (value != _value) {
        Q_EMIT valueChanged(_value = value);
        checkAlert();
    }
}

void Sensor::checkAlert()
{
    const auto alert = (_properties.enabled && _properties.alertsEnabled && (_value >= _properties.alertsThreshold));
    if (alert != _alert)
        Q_EMIT alertChanged(_alert = alert);
}

// ChipData

Chip::Chip(const ChipData &data, QObject *parent)
    : QObject(parent)
    , _data(data)
{
    static auto sensorName = [] (const sensors_chip_name *chip, const sensors_feature *feature) -> QString {
        Q_CHECK_PTR(chip);
        Q_CHECK_PTR(feature);
        QScopedPointer<char, QScopedPointerPodDeleter> name(sensors_get_label(chip, feature));
        if (name)
            return name.data();

        return feature->name;
    };

    const sensors_feature *feature = Q_NULLPTR;
    auto i = 0;

    Q_CHECK_PTR(_data.chip);
    while (Q_NULLPTR != (feature = sensors_get_features(_data.chip, &i))) {
        if (SENSORS_FEATURE_TEMP != feature->type)
            continue;

        auto subfeature = sensors_get_subfeature(_data.chip, feature, SENSORS_SUBFEATURE_TEMP_INPUT);
        if (Q_NULLPTR == subfeature)
            continue;

        const SensorData data = {
            _data.chip,
            subfeature->number,
            sensorName(_data.chip, feature),
            QString("%1/%2").arg(_data.id).arg(data.name)
        };

        _sensors << new Sensor(data, this);
    }
}

// LmSensors

LmSensors::LmSensors()
    : QObject()
{
    if (0 != sensors_init(Q_NULLPTR)) {
        qCritical("Failed to initialize libsensors");
        return;
    }

    static auto chipName = [] (const sensors_chip_name *chip) -> QString {
        Q_CHECK_PTR(chip);
        auto count = sensors_snprintf_chip_name(Q_NULLPTR, 0, chip);
        if (count > 0) {
            QScopedArrayPointer<char> name(new char[++count]);
            if (sensors_snprintf_chip_name(name.data(), count, chip) > 0)
                return name.data();
        }

        return chip->prefix;
    };

    const sensors_chip_name *chip = Q_NULLPTR;
    auto i = 0;
    while (Q_NULLPTR != (chip = sensors_get_detected_chips(Q_NULLPTR, &i))) {
        const ChipData data = {
            chip,
            chipName(chip),
            QString("lmsensors/%1").arg(data.name)
        };

        _chips << new Chip(data, this);
    }
}

const ChipList& LmSensors::chips()
{
    static LmSensors instance;

    return instance._chips;
}
