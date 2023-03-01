#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "machine.h"
#include "scanscreen.h"
#include "standbyscreen.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void exit();
    void rescan();
    void onDeviceStatusChanged(QString type);
    void onScanStatusChanged(QString id, QString type);

private:
    void runMachine();
    void connectWidgets();


    Ui::MainWindow *ui;
    Machine* m_machine;
    ScanScreen m_scanscreen;
    StandByScreen m_stanbyscreen;
};
#endif // MAINWINDOW_H
