#pragma once
#include <QMainWindow>
#include <QDialog>
#include <iostream>
#include "ui_secondWindow.h"
using namespace std;

class secondWindow : public QDialog
{
	Q_OBJECT

public:
	secondWindow(QWidget *parent = nullptr);
	~secondWindow();

private:
	Ui::secondWindowClass ui;

private slots:
	void on_btnTxtArchive_clicked();	
	void on_btnGenerar_clicked();
};

