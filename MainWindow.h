#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"

#include <QPointer>

class QSoundEffect;
class QSystemTrayIcon;
class QTimer;

class AlertsWatcher;
class Sensor;
class Settings;

// MainWindow
class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void initPlot();
    void initSensors();
    void initSound();
    void initTimer();
    void initTrayIcon();
    void initWatcher();

    void showAlertMessage();
    void updateSensors();

    static const QString _keyGeometry;
    static const QString _keySensorColor;
    static const QString _keySensorEnabled;
    static const QString _keySensorAlertsEnabled;
    static const QString _keySensorAlertsThreshold;

    const QPixmap _pixmapBlue;
    const QPixmap _pixmapRed;

    QPointer<QSystemTrayIcon> _trayIcon;
    QPointer<QTimer> _timer;
    QPointer<Settings> _settings;
    QPointer<QSoundEffect> _sound;
    QPointer<AlertsWatcher> _watcher;
    QHash<Sensor*, PlotGraph*> _graphs;

    // GUI
    Q_SLOT void on_actionExit_triggered();
    Q_SLOT void on_actionSettings_triggered();
    Q_SLOT void on_actionAbout_triggered();
    Q_SLOT void on_actionShow_triggered();
};

#endif // MAINWINDOW_H
