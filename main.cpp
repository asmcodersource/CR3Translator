#include "listwidget.h"
#include "mainwidget.h"
#include "mainwindow.h"
#include "settingswidget.h"
#include <QMessageBox>
#include <QApplication>
// Розробка програмного забезпечення для перекладу YML файлів.


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(800, 600);
    w.show();
    return a.exec();
}
