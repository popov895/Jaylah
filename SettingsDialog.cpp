#include "SettingsDialog.h"

#include "SensorPropertiesDialog.h"
#include "Settings.h"

// SettingsDialog

SettingsDialog::SettingsDialog(Settings *settings, QWidget *parent)
    : QDialog(parent)
    , _settings(settings)
{
    Q_ASSERT(_settings);

    setupUi(this);

    spinBoxUpdateInterval->setMinimum(Settings::minUpdateInterval);
    spinBoxUpdateInterval->setValue(_settings->updateInterval());

    spinBoxDisplayInterval->setMinimum(Settings::minDisplayInterval);
    spinBoxDisplayInterval->setValue(_settings->displayInterval());

    checkBoxShowMessages->setChecked(_settings->showMessages());
    checkBoxPlaySound->setChecked(_settings->playSound());

    treeWidgetSensors->expandAll();
}

void SettingsDialog::applyChanges()
{
    treeWidgetSensors->applyChanges();

    _settings->setUpdateInterval(spinBoxUpdateInterval->value());
    _settings->setDisplayInterval(spinBoxDisplayInterval->value());
    _settings->setShowMessages(checkBoxShowMessages->isChecked());
    _settings->setPlaySound(checkBoxPlaySound->isChecked());
}

void SettingsDialog::on_treeWidgetSensors_currentItemChanged(QTreeWidgetItem *current)
{
    pushButtonProperties->setEnabled((Q_NULLPTR != current) && (current->flags() & Qt::ItemIsSelectable));
}

void SettingsDialog::on_treeWidgetSensors_itemDoubleClicked(QTreeWidgetItem *item)
{
    if ((Q_NULLPTR != item) && (item->flags() & Qt::ItemIsSelectable))
        pushButtonProperties->click();
}

void SettingsDialog::on_pushButtonProperties_clicked()
{
    auto item = dynamic_cast<SensorTreeWidgetItem*>(treeWidgetSensors->currentItem());
    Q_CHECK_PTR(item);
    SensorPropertiesDialog(item, this).exec();
}

void SettingsDialog::on_pushButtonOk_clicked()
{
    applyChanges();
    accept();
}

void SettingsDialog::on_pushButtonApply_clicked()
{
    applyChanges();
}
