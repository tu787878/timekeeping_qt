
#include <QApplication>
#include "machine.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow wnd;
    wnd.show();
    return a.exec();
}
