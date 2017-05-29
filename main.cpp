#include <QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setApplicationName("Jaylah");
    application.setOrganizationName("popov895");

    MainWindow window;
    window.show();

    return application.exec();
}
