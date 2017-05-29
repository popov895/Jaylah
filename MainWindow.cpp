#include "MainWindow.h"

#include <QCloseEvent>
#include <QSoundEffect>
#include <QSystemTrayIcon>
#include <QTimer>

#include "AboutDialog.h"
#include "AlertsWatcher.h"
#include "LmSensors.h"
#include "Settings.h"
#include "SettingsDialog.h"

// MainWindow

const QString MainWindow::_keyGeometry = "geometry";
const QString MainWindow::_keySensorColor = "%1/color";
const QString MainWindow::_keySensorEnabled = "%1/enabled";
const QString MainWindow::_keySensorAlertsEnabled = "%1/alertsEnabled";
const QString MainWindow::_keySensorAlertsThreshold = "%1/alertsThreshold";

MainWindow::MainWindow()
    : QMainWindow()
    , _pixmapBlue(":/resources/icons/trayicon.svgz")
    , _pixmapRed(":/resources/icons/trayicon_red.svgz")
    , _trayIcon(new QSystemTrayIcon(this))
    , _timer(new QTimer(this))
    , _settings(new Settings(this))
    , _sound(new QSoundEffect(this))
    , _watcher(new AlertsWatcher(this))
{
    setupUi(this);
    setWindowTitle(QApplication::applicationName());

    restoreGeometry(_settings->value(_keyGeometry).toByteArray());

    initPlot();
    initTrayIcon();
    initSound();
    initSensors();
    initWatcher();
    initTimer();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    hide();
}

void MainWindow::initPlot()
{
    plot->setDisplayInterval(_settings->displayInterval());

    connect(_settings, &Settings::displayIntervalChanged, [this] (const int displayInterval) {
        plot->setDisplayInterval(displayInterval);
    });
}

void MainWindow::initSensors()
{
    for (auto chip : LmSensors::chips()) {
        for (auto sensor : chip->sensors()) {
            const auto id = sensor->id();

            SensorProperties properties = sensor->properties();
            properties.enabled = _settings->value(_keySensorEnabled.arg(id), properties.enabled).toBool();
            properties.color = _settings->value(_keySensorColor.arg(id), properties.color).value<QColor>();
            properties.alertsEnabled = _settings->value(_keySensorAlertsEnabled.arg(id), properties.alertsEnabled).toBool();
            properties.alertsThreshold = qMax(0, _settings->value(_keySensorAlertsThreshold.arg(id), properties.alertsThreshold).toInt());
            sensor->setProperties(properties);

            connect(sensor, &Sensor::colorChanged, [this, sensor, id] (const QColor &color) {
                if (sensor->properties().enabled) {
                    Q_ASSERT(_graphs.contains(sensor));
                    _graphs[sensor]->setColor(color);
                }
                _settings->setValue(_keySensorColor.arg(id), color);
            });

            connect(sensor, &Sensor::enabledChanged, [this, sensor, id] (const bool enabled) {
                if (enabled) {
                    Q_ASSERT(!_graphs.contains(sensor));
                    sensor->update();
                    auto graph = plot->addGraph(sensor->name(), sensor->properties().color);
                    graph->addY(sensor->value());
                    _graphs[sensor] = graph;
                } else {
                    Q_ASSERT(_graphs.contains(sensor));
                    delete _graphs.take(sensor);
                }
                _settings->setValue(_keySensorEnabled.arg(id), enabled);
            });

            connect(sensor, &Sensor::alertsEnabledChanged, [this, id] (const bool alertsEnabled) {
                _settings->setValue(_keySensorAlertsEnabled.arg(id), alertsEnabled);
            });

            connect(sensor, &Sensor::alertsThresholdChanged, [this, id] (const int alertsThreshold) {
                _settings->setValue(_keySensorAlertsThreshold.arg(id), alertsThreshold);
            });

            if (sensor->properties().enabled)
                _graphs[sensor] = plot->addGraph(sensor->name(), sensor->properties().color);
        }
    }

    updateSensors();
}

void MainWindow::initSound()
{
    _sound->setLoopCount(QSoundEffect::Infinite);
    _sound->setSource(QUrl::fromLocalFile(":/resources/sound/alert.wav"));
}

void MainWindow::initTimer()
{
    connect(_timer, &QTimer::timeout, [this] () {
        updateSensors();
    });

    connect(_settings, &Settings::updateIntervalChanged, [this] (const int updateInterval) {
        _timer->stop();
        const auto updateIntervalMs = 1000 * updateInterval;

        QTimer::singleShot(qMax(0, updateIntervalMs - _timer->interval() + _timer->remainingTime()), [this, updateIntervalMs] () {
            updateSensors();
            _timer->start(updateIntervalMs);
        });
    });

    _timer->start(1000 * _settings->updateInterval());
}

void MainWindow::initTrayIcon()
{
    auto menu = new QMenu(this);
    menu->addAction(actionShow);
    menu->addSeparator();
    menu->addAction(actionExit);

    _trayIcon->setContextMenu(menu);
    _trayIcon->setIcon(_pixmapBlue);
    _trayIcon->setToolTip(windowTitle());

    connect(_trayIcon, &QSystemTrayIcon::activated, [this] (QSystemTrayIcon::ActivationReason reason) {
        if (QSystemTrayIcon::Trigger == reason)
            actionShow->trigger();
    });

    connect(_trayIcon, &QSystemTrayIcon::messageClicked, [this] () {
        actionShow->trigger();
    });

    _trayIcon->show();
}

void MainWindow::initWatcher()
{
    auto doAlert = [this] () {
        _trayIcon->setIcon(_pixmapRed);
        if (_settings->showMessages())
            showAlertMessage();
        if (_settings->playSound())
            _sound->play();
    };

    connect(_watcher, &AlertsWatcher::alertChanged, [this, doAlert] (const bool alert) {
        if (alert) {
            doAlert();
        } else {
            _sound->stop();
            _trayIcon->setIcon(_pixmapBlue);
        }
    });

    connect(_watcher, &AlertsWatcher::sensorChanged, [this] () {
        if (_settings->showMessages() && _watcher->alert())
            showAlertMessage();
    });

    connect(_settings, &Settings::showMessagesChanged, [this] (const bool showMessage) {
        if (showMessage && _watcher->alert())
            showAlertMessage();
    });

    connect(_settings, &Settings::playSoundChanged, [this] (const bool playSound) {
        if (!playSound)
            _sound->stop();
        else if (_watcher->alert())
            _sound->play();
    });

    if (_watcher->alert())
        doAlert();
}

void MainWindow::showAlertMessage()
{
    const auto sensor = _watcher->sensor();
    Q_CHECK_PTR(sensor);
    _trayIcon->showMessage("Temperature alert", QString("%1 : %2 °C").arg(sensor->name()).arg(sensor->value()), QSystemTrayIcon::Warning);
}

void MainWindow::updateSensors()
{
    static constexpr auto maxX = std::numeric_limits<int>::max();
    static int x = 0;

    for (auto i = _graphs.begin(); i != _graphs.end(); ++i) {
        i.key()->update();
        i.value()->addXY(x, i.key()->value());
    }
    (maxX == x) ? (x = 0) : ++x;
}

void MainWindow::on_actionExit_triggered()
{
    _settings->setValue(_keyGeometry, saveGeometry());

    QApplication::quit();
}

void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog(_settings, this).exec();
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_actionShow_triggered()
{
    show();
    activateWindow();
}
