#include "SensorsTreeWidget.h"

// SensorTreeWidgetItem

SensorTreeWidgetItem::SensorTreeWidgetItem(Sensor *sensor)
    : QTreeWidgetItem()
    , _sensor(sensor)
{
    setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemNeverHasChildren);

    Q_ASSERT(_sensor);
    _properties = _sensor->properties();
}

QVariant SensorTreeWidgetItem::data(int column, int role) const
{
    Q_ASSERT(_sensor);

    if (0 == column) {
        switch (role) {
            case Qt::CheckStateRole:
                return (_properties.enabled ? Qt::Checked : Qt::Unchecked);
            case Qt::DisplayRole:
                return _sensor->name();
            default:
                break;
        }
    }

    return QTreeWidgetItem::data(column, role);
}

void SensorTreeWidgetItem::setData(int column, int role, const QVariant &value)
{
    if ((0 == column) && (Qt::CheckStateRole == role)) {
        _properties.enabled = (Qt::Checked == value.value<Qt::CheckState>());
        emitDataChanged();
    }

    QTreeWidgetItem::setData(column, role, value);
}

void SensorTreeWidgetItem::applyChanges()
{
    Q_ASSERT(_sensor);
    _sensor->setProperties(_properties);
}

// ChipTreeWidgetItem

ChipTreeWidgetItem::ChipTreeWidgetItem(Chip *chip)
    : QTreeWidgetItem()
    , _chip(chip)
{
    setFlags(Qt::ItemIsEnabled);
}

QVariant ChipTreeWidgetItem::data(int column, int role) const
{
    Q_ASSERT(_chip);

    if ((0 == column) && (Qt::DisplayRole == role))
        return _chip->name();

    return QTreeWidgetItem::data(column, role);
}

void ChipTreeWidgetItem::applyChanges()
{
    for (auto i = 0; i < childCount(); ++i) {
        auto item = dynamic_cast<SensorTreeWidgetItem*>(child(i));
        Q_CHECK_PTR(item);
        item->applyChanges();
    }
}

// SensorsTreeWidget

SensorsTreeWidget::SensorsTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    for (auto chip : LmSensors::chips()) {
        auto item = new ChipTreeWidgetItem(chip);
        for (auto sensor : chip->sensors())
            item->addChild(new SensorTreeWidgetItem(sensor));
        addTopLevelItem(item);
    }
}

void SensorsTreeWidget::applyChanges()
{
    for (auto i = 0; i < topLevelItemCount(); ++i) {
        auto item = dynamic_cast<ChipTreeWidgetItem*>(topLevelItem(i));
        Q_CHECK_PTR(item);
        item->applyChanges();
    }
}
