#ifndef STANDBYSCREEN_H
#define STANDBYSCREEN_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class StandByScreen;
}

class StandByScreen : public QWidget
{
    Q_OBJECT

public:
    explicit StandByScreen(QWidget *parent = nullptr);
    ~StandByScreen();
    void setDevices(QStringList data);
    void setMessageError(QString error);

private slots:
    void on_closeWindow_clicked();
    void showTime();
    void resetMessage();
    void on_pushButton_clicked();

signals:
    void exit();
    void rescan();

private:
    Ui::StandByScreen *ui;
    QTimer* m_timer_message;
};

#endif // STANDBYSCREEN_H
