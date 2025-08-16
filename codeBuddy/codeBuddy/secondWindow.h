#pragma once

#include <QMainWindow>
#include <QDialog>
#include "ui_secondWindow.h"

class secondWindow : public QDialog
{
	Q_OBJECT

public:
	secondWindow(QWidget *parent = nullptr);
	~secondWindow();

private:
	Ui::secondWindowClass ui;

private slots: //slots son las declaraciones de acciones o metodos de accion de mis nodos :)
	void on_btnTxtArchive_clicked();	
};

