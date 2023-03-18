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
    void setSettings(QString logo, QString checkInColor, QString checkOutColor);
    void setIsCheckin(bool isCheckIn);
signals:
    void backToStandBy();
    void getCalendar();
private slots:
    void on_backButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::ScanScreen *ui;
    QString m_checkin_color = "rgb(0, 255, 0)";
    QString m_checkout_color = "rgb(0, 255, 0)";
    QString m_logi_url = "/home/pi/Downloads/SmallLogoBW.png";
    bool m_isCheckin = true;
};

#endif // SCANSCREEN_H
