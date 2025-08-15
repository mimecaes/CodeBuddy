/********************************************************************************
** Form generated from reading UI file 'mainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainWindowClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *labelTittle;
    QLabel *LabelBuddy;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnInicio;
    QPushButton *btnSalir;
    QTextEdit *txtSobreApp;

    void setupUi(QMainWindow *mainWindowClass)
    {
        if (mainWindowClass->objectName().isEmpty())
            mainWindowClass->setObjectName("mainWindowClass");
        mainWindowClass->resize(1280, 862);
        centralWidget = new QWidget(mainWindowClass);
        centralWidget->setObjectName("centralWidget");
        centralWidget->setStyleSheet(QString::fromUtf8("background-color: #f9f9f0;"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(16);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setSizeConstraint(QLayout::SizeConstraint::SetDefaultConstraint);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        labelTittle = new QLabel(centralWidget);
        labelTittle->setObjectName("labelTittle");
        labelTittle->setEnabled(true);
        labelTittle->setPixmap(QPixmap(QString::fromUtf8("../../../../../Pictures/Screenshots/Captura de pantalla 2025-08-14 135404.png")));
        labelTittle->setScaledContents(true);

        verticalLayout->addWidget(labelTittle, 0, Qt::AlignmentFlag::AlignHCenter);

        LabelBuddy = new QLabel(centralWidget);
        LabelBuddy->setObjectName("LabelBuddy");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LabelBuddy->sizePolicy().hasHeightForWidth());
        LabelBuddy->setSizePolicy(sizePolicy);
        LabelBuddy->setMaximumSize(QSize(250, 250));
        LabelBuddy->setPixmap(QPixmap(QString::fromUtf8("../../../../../Pictures/Screenshots/Captura de pantalla 2025-08-14 140248.png")));
        LabelBuddy->setScaledContents(true);
        LabelBuddy->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(LabelBuddy, 0, Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignVCenter);

        widget = new QWidget(centralWidget);
        widget->setObjectName("widget");
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(9, -1, -1, -1);
        btnInicio = new QPushButton(widget);
        btnInicio->setObjectName("btnInicio");
        btnInicio->setMaximumSize(QSize(500, 16777215));
        btnInicio->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #ffd2e7;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    padding: 10px 20px;\n"
"    font-size: 14pt;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #ffa8d5;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #ffd2e7;\n"
"}\n"
""));

        horizontalLayout->addWidget(btnInicio);

        btnSalir = new QPushButton(widget);
        btnSalir->setObjectName("btnSalir");
        btnSalir->setMaximumSize(QSize(500, 16777215));
        btnSalir->setAcceptDrops(false);
        btnSalir->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #cfe7b7;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    padding: 10px 20px;\n"
"    font-size: 14pt;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #bae093;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #cfe7b7;\n"
"}\n"
""));
        btnSalir->setAutoDefault(false);
        btnSalir->setFlat(false);

        horizontalLayout->addWidget(btnSalir);


        verticalLayout->addWidget(widget, 0, Qt::AlignmentFlag::AlignHCenter);


        verticalLayout_2->addLayout(verticalLayout);

        txtSobreApp = new QTextEdit(centralWidget);
        txtSobreApp->setObjectName("txtSobreApp");
        txtSobreApp->setMaximumSize(QSize(16777215, 115));
        txtSobreApp->setStyleSheet(QString::fromUtf8("QTextEdit {\n"
"    background: #fcf0f4;\n"
"    color: #3a3a3a;\n"
"    border: 3px solid #ffd2e7;\n"
"    border-radius: 12px;\n"
"    padding: 12px 16px;\n"
"    font-size: 12pt;\n"
"    selection-background-color: #ead1f6;\n"
"    selection-color: #2c2c2c;\n"
"}\n"
"\n"
"QTextEdit:disabled {\n"
"    background: #f4f4f4;\n"
"    color: #9a9a9a;\n"
"    border-color: #dddddd;\n"
"}"));

        verticalLayout_2->addWidget(txtSobreApp);

        mainWindowClass->setCentralWidget(centralWidget);

        retranslateUi(mainWindowClass);
        QObject::connect(btnSalir, &QPushButton::clicked, mainWindowClass, qOverload<>(&QMainWindow::close));

        btnSalir->setDefault(false);


        QMetaObject::connectSlotsByName(mainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *mainWindowClass)
    {
        mainWindowClass->setWindowTitle(QCoreApplication::translate("mainWindowClass", "mainWindowController", nullptr));
        labelTittle->setText(QString());
        LabelBuddy->setText(QString());
        btnInicio->setText(QCoreApplication::translate("mainWindowClass", "\313\232\342\202\212\342\200\247\352\222\260\341\203\220 Iniciar \340\273\222\352\222\261 \342\200\247\342\202\212\313\232", nullptr));
        btnSalir->setText(QCoreApplication::translate("mainWindowClass", "\313\232\342\202\212\342\200\247\352\222\260\341\203\220 Salir \340\273\222\352\222\261 \342\200\247\342\202\212\313\232", nullptr));
        txtSobreApp->setHtml(QCoreApplication::translate("mainWindowClass", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:12pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:700;\">Code Buddy</span> transforma tus instrucciones escritas en un archivo <span style=\" font-weight:700;\">.txt</span> en <span style=\" font-weight:700;\">c\303\263digo C++</span> funcional. Solo selecciona tu archivo: la app lo lee, entiende lo que pides y te devuelve el c\303\263digo C++ correspondiente para que lo revises, compiles y ejecutes.</p>\n"
""
                        "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\342\217\224\342\217\224\342\217\224 \352\222\260 \341\247\224\340\267\206\341\247\223 \352\222\261 \342\217\224\342\217\224\342\217\224</p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class mainWindowClass: public Ui_mainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
