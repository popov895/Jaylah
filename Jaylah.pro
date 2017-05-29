QT += \
    core \
    gui \
    widgets \
    charts \
    multimedia

TARGET = Jaylah

TEMPLATE = app

DEFINES += \
    QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    SettingsDialog.cpp \
    SensorPropertiesDialog.cpp \
    Plot.cpp \
    LmSensors.cpp \
    SensorsTreeWidget.cpp \
    Settings.cpp \
    AlertsWatcher.cpp \
    ColorComboBox.cpp \
    AboutDialog.cpp

HEADERS += \
    MainWindow.h \
    SettingsDialog.h \
    SensorPropertiesDialog.h \
    Plot.h \
    LmSensors.h \
    SensorsTreeWidget.h \
    Settings.h \
    AlertsWatcher.h \
    ColorComboBox.h \
    AboutDialog.h

FORMS += \
    MainWindow.ui \
    SettingsDialog.ui \
    SensorPropertiesDialog.ui \
    AboutDialog.ui

RESOURCES += \
    resources.qrc

LIBS += \
    -lsensors

!unix:message("This application supports Linux only")
