/********************************************************************************
** Form generated from reading UI file 'secondWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SECONDWINDOW_H
#define UI_SECONDWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_secondWindowClass
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *principalLayout;
    QHBoxLayout *topLayout;
    QPushButton *btnVolver;
    QLabel *labelTxt;
    QSpacerItem *horizontalSpacer_2;
    QLabel *labelCode;
    QHBoxLayout *windowsLayout;
    QPlainTextEdit *TxtWindow;
    QPlainTextEdit *codeWindow;
    QHBoxLayout *btnsLayout;
    QPushButton *btnTxtArchive;
    QPushButton *btnGenerar;

    void setupUi(QWidget *secondWindowClass)
    {
        if (secondWindowClass->objectName().isEmpty())
            secondWindowClass->setObjectName("secondWindowClass");
        secondWindowClass->setEnabled(true);
        secondWindowClass->resize(1276, 854);
        secondWindowClass->setMaximumSize(QSize(16777215, 16777215));
        secondWindowClass->setMouseTracking(false);
        secondWindowClass->setTabletTracking(false);
        secondWindowClass->setAcceptDrops(false);
        secondWindowClass->setAutoFillBackground(false);
        secondWindowClass->setStyleSheet(QString::fromUtf8("background-color: #f9f9f0;\n"
""));
        verticalLayout_2 = new QVBoxLayout(secondWindowClass);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName("verticalLayout_2");
        principalLayout = new QVBoxLayout();
        principalLayout->setSpacing(6);
        principalLayout->setObjectName("principalLayout");
        topLayout = new QHBoxLayout();
        topLayout->setSpacing(6);
        topLayout->setObjectName("topLayout");
        btnVolver = new QPushButton(secondWindowClass);
        btnVolver->setObjectName("btnVolver");
        btnVolver->setEnabled(true);
        btnVolver->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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

        topLayout->addWidget(btnVolver);

        labelTxt = new QLabel(secondWindowClass);
        labelTxt->setObjectName("labelTxt");
        labelTxt->setStyleSheet(QString::fromUtf8(" QLabel { color: #ffffff; font-size: 20px; font-weight: bold; font-family: \"Segoe UI\", sans-serif; background-color: #dfe7fc; border: 2px solid #cddafd; border-radius: 8px; padding: 6px 12px; qproperty-alignment: AlignCenter; }"));

        topLayout->addWidget(labelTxt);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        topLayout->addItem(horizontalSpacer_2);

        labelCode = new QLabel(secondWindowClass);
        labelCode->setObjectName("labelCode");
        labelCode->setStyleSheet(QString::fromUtf8(" QLabel { color: #ffffff; font-size: 20px; font-weight: bold; font-family: \"Segoe UI\", sans-serif; background-color: #dfe7fc; border: 2px solid #cddafd; border-radius: 8px; padding: 6px 12px; qproperty-alignment: AlignCenter; }"));

        topLayout->addWidget(labelCode);


        principalLayout->addLayout(topLayout);

        windowsLayout = new QHBoxLayout();
        windowsLayout->setSpacing(6);
        windowsLayout->setObjectName("windowsLayout");
        TxtWindow = new QPlainTextEdit(secondWindowClass);
        TxtWindow->setObjectName("TxtWindow");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TxtWindow->sizePolicy().hasHeightForWidth());
        TxtWindow->setSizePolicy(sizePolicy);

        windowsLayout->addWidget(TxtWindow);

        codeWindow = new QPlainTextEdit(secondWindowClass);
        codeWindow->setObjectName("codeWindow");
        sizePolicy.setHeightForWidth(codeWindow->sizePolicy().hasHeightForWidth());
        codeWindow->setSizePolicy(sizePolicy);

        windowsLayout->addWidget(codeWindow);


        principalLayout->addLayout(windowsLayout);

        btnsLayout = new QHBoxLayout();
        btnsLayout->setSpacing(180);
        btnsLayout->setObjectName("btnsLayout");
        btnsLayout->setContentsMargins(100, -1, 100, -1);
        btnTxtArchive = new QPushButton(secondWindowClass);
        btnTxtArchive->setObjectName("btnTxtArchive");
        btnTxtArchive->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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

        btnsLayout->addWidget(btnTxtArchive);

        btnGenerar = new QPushButton(secondWindowClass);
        btnGenerar->setObjectName("btnGenerar");
        btnGenerar->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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

        btnsLayout->addWidget(btnGenerar);


        principalLayout->addLayout(btnsLayout);


        verticalLayout_2->addLayout(principalLayout);


        retranslateUi(secondWindowClass);

        QMetaObject::connectSlotsByName(secondWindowClass);
    } // setupUi

    void retranslateUi(QWidget *secondWindowClass)
    {
        secondWindowClass->setWindowTitle(QCoreApplication::translate("secondWindowClass", "PantallaPrincipal", nullptr));
        btnVolver->setText(QCoreApplication::translate("secondWindowClass", "\342\244\266", nullptr));
        labelTxt->setText(QCoreApplication::translate("secondWindowClass", "Archivo txt", nullptr));
        labelCode->setText(QCoreApplication::translate("secondWindowClass", "Codigo generado", nullptr));
        btnTxtArchive->setText(QCoreApplication::translate("secondWindowClass", "\313\232\342\202\212\342\200\247\352\222\260\341\203\220 Cargar archivo \340\273\222\352\222\261 \342\200\247\342\202\212\313\232", nullptr));
        btnGenerar->setText(QCoreApplication::translate("secondWindowClass", "\313\232\342\202\212\342\200\247\352\222\260\341\203\220 Generar Codigo \340\273\222\352\222\261 \342\200\247\342\202\212\313\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class secondWindowClass: public Ui_secondWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SECONDWINDOW_H
