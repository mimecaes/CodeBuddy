#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainWindow.h"
#include "secondWindow.h"


class mainWindow : public QMainWindow
{
    Q_OBJECT

public:
    mainWindow(QWidget *parent = nullptr);
    ~mainWindow();

private:
    Ui::mainWindowClass ui;
    secondWindow* window;

private slots:
    void on_btnInicio_clicked();
};

