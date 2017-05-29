#include "SensorPropertiesDialog.h"

#include "SensorsTreeWidget.h"

// SensorPropertiesDialog

SensorPropertiesDialog::SensorPropertiesDialog(SensorTreeWidgetItem *item, QWidget *parent)
    : QDialog(parent)
    , _item(item)
{
    setupUi(this);

    Q_CHECK_PTR(_item);
    comboBoxColors->setCurrentColor(item->color());
    groupBoxAlerts->setChecked(item->alertsEnabled());
    spinBoxThreshold->setValue(item->alertsThreshold());
}

void SensorPropertiesDialog::on_pushButtonOk_clicked()
{
    Q_CHECK_PTR(_item);
    _item->setColor(comboBoxColors->currentColor());
    _item->setAlertsEnabled(groupBoxAlerts->isChecked());
    _item->setAlertsThreshold(spinBoxThreshold->value());

    accept();
}
