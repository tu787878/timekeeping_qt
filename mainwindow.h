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
    void onScanFail(QString error);
    void onScanSuccess(QString name, QString user_id, QString time);
    void toStandBy();
    void onGetCalendar();
    void onReceivedCalendar();
    void setStatusBarText(QString text, QString rgb);

private:
    void runMachine();
    void connectWidgets();


    Ui::MainWindow *ui;
    Machine* m_machine;
    ScanScreen m_scanscreen;
    StandByScreen m_stanbyscreen;
    QString m_current_user_id = "";
    QTimer* m_timer;

};
#endif // MAINWINDOW_H
