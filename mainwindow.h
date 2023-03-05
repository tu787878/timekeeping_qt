#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "machine.h"
#include "scanscreen.h"
#include "standbyscreen.h"
#include <QJsonArray>

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
    void onDeviceStatusChanged(QStringList data);
    void onScanFail(QString error);
    void onScanSuccess(QString name, QString time, QString type, QString userid);
    void toStandBy();
    void onGetCalendar();
    void onReceivedCalendar(QJsonArray jsonArray);
    void onReceivedCalendarFail(QString error);
    void setStatusBarText(QString text, QString rgb);
    void resetStatus();

private:
    void runMachine();
    void connectWidgets();


    Ui::MainWindow *ui;
    Machine* m_machine;
    ScanScreen m_scanscreen;
    StandByScreen m_stanbyscreen;
    QString m_current_user_id = "";
    QTimer* m_timer;
    QTimer* m_timer_status;

};
#endif // MAINWINDOW_H
