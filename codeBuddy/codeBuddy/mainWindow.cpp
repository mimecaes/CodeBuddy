#include "mainWindow.h"
#include "ui_mainWindow.h"
#include "secondWindow.h"

mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

mainWindow::~mainWindow()
{}

void mainWindow::on_btnInicio_clicked() {
    this->hide();              
    secondWindow dlg(this);       
    int r = dlg.exec();         
    this->show();
}
