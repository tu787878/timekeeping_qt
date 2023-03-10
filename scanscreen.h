#ifndef SCANSCREEN_H
#define SCANSCREEN_H

#include <QWidget>
#include <QJsonArray>
#include <QJsonObject>

namespace Ui {
class ScanScreen;
}

class ScanScreen : public QWidget
{
    Q_OBJECT

public:
    explicit ScanScreen(QWidget *parent = nullptr);
    ~ScanScreen();
    void setNameLabel(QString name);
    void setTimeLabel(QString time);
    void showCalendar();
    void hideCalendar();
    void setItems(QJsonArray array);
signals:
    void backToStandBy();
    void getCalendar();
private slots:
    void on_backButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::ScanScreen *ui;
};

#endif // SCANSCREEN_H
