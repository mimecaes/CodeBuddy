#include "mainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    mainWindow window;
    window.show();
    return app.exec();
}
