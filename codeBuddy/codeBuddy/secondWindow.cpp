#include "secondWindow.h"

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QStandardPaths>
#include <QFileInfo>
#include <QFontDatabase> 

secondWindow::secondWindow(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

secondWindow::~secondWindow()
{}

void secondWindow::on_btnTxtArchive_clicked()
{
    static QString lastDir =
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation); //idea: recordar el ultimo path e iniciar ahi

    const QString path = QFileDialog::getOpenFileName(
        this,
        tr("Abrir archivo de texto"),
        lastDir,
        tr("Archivos de texto (*.txt);;Todos los archivos (*.*)")
    );
    if (path.isEmpty())
        return;

    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"),
            tr("No se pudo abrir \"%1\":\n%2")
            .arg(QFileInfo(path).fileName(), f.errorString()));
        return;
    }

    QTextStream in(&f); //lector del archivo

    #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        in.setEncoding(QStringConverter::Utf8);
    #else
        in.setCodec("UTF-8");
    #endif

    const QString texto = in.readAll();

    ui.TxtWindow->setPlainText(texto);

    ui.TxtWindow->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    ui.TxtWindow->setLineWrapMode(QPlainTextEdit::NoWrap);

    lastDir = QFileInfo(path).absolutePath();
}
