#ifndef STANDBYSCREEN_H
#define STANDBYSCREEN_H

#include <QWidget>

namespace Ui {
class StandByScreen;
}

class StandByScreen : public QWidget
{
    Q_OBJECT

public:
    explicit StandByScreen(QWidget *parent = nullptr);
    ~StandByScreen();

private slots:
    void on_closeWindow_clicked();

    void on_pushButton_clicked();

signals:
    void exit();
    void rescan();

private:
    Ui::StandByScreen *ui;
};

#endif // STANDBYSCREEN_H
