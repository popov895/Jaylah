#ifndef SENSORPROPERTIESDIALOG_H
#define SENSORPROPERTIESDIALOG_H

#include "ui_SensorPropertiesDialog.h"

class SensorTreeWidgetItem;

// SensorPropertiesDialog
class SensorPropertiesDialog : public QDialog, private Ui::SensorPropertiesDialog
{
    Q_OBJECT

public:
    explicit SensorPropertiesDialog(SensorTreeWidgetItem *item, QWidget *parent);

private:
    SensorTreeWidgetItem *_item;

    // GUI
    Q_SLOT void on_pushButtonOk_clicked();
};

#endif // SENSORPROPERTIESDIALOG_H
