#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "ui_SettingsDialog.h"

#include <QPointer>

class Settings;

// SettingsDialog
class SettingsDialog : public QDialog, private Ui::SettingsDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(Settings *settings, QWidget *parent);

private:
    void applyChanges();

    QPointer<Settings> _settings;

    // GUI
    Q_SLOT void on_treeWidgetSensors_currentItemChanged(QTreeWidgetItem *current);
    Q_SLOT void on_treeWidgetSensors_itemDoubleClicked(QTreeWidgetItem *item);
    Q_SLOT void on_pushButtonProperties_clicked();
    Q_SLOT void on_pushButtonOk_clicked();
    Q_SLOT void on_pushButtonApply_clicked();
};

#endif // SETTINGSDIALOG_H
