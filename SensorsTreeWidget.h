#ifndef SENSORSTREEWIDGET_H
#define SENSORSTREEWIDGET_H

#include <QPointer>
#include <QTreeWidget>

#include "LmSensors.h"

// SensorTreeWidgetItem
class SensorTreeWidgetItem : public QTreeWidgetItem
{
    friend class SensorsTreeWidget;

private:
    SensorTreeWidgetItem(Sensor *sensor);
    virtual ~SensorTreeWidgetItem() {}

public:
    QVariant data(int column, int role) const override;
    void setData(int column, int role, const QVariant &value) override;

    const QColor& color() const { return _properties.color; }
    void setColor(const QColor &color) { _properties.color = color; }

    bool alertsEnabled () const { return _properties.alertsEnabled; }
    void setAlertsEnabled (const bool alertsEnabled) { _properties.alertsEnabled = alertsEnabled; }

    int alertsThreshold() const { return _properties.alertsThreshold; }
    void setAlertsThreshold(const int alertsThreshold) { _properties.alertsThreshold = alertsThreshold; }

    void applyChanges();

private:
    QPointer<Sensor> _sensor;
    SensorProperties _properties;
};

// ChipTreeWidgetItem
class ChipTreeWidgetItem : public QTreeWidgetItem
{
    friend class SensorsTreeWidget;

private:
    ChipTreeWidgetItem(Chip *chip);
    virtual ~ChipTreeWidgetItem() {}

public:
    QVariant data(int column, int role) const override;

    void applyChanges();

private:
    QPointer<Chip> _chip;
};

// SensorsTreeWidget
class SensorsTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    explicit SensorsTreeWidget(QWidget *parent = Q_NULLPTR);

    void applyChanges();
};

#endif // SENSORSTREEWIDGET_H
