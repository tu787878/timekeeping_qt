#ifndef MACHINE_H
#define MACHINE_H

#include "iscandevice.h"
#include "apihandle.h"
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

class Machine : public QObject
{
Q_OBJECT
public:
    Machine(QObject* parent = nullptr);
    ~Machine();

    void startScan();
    void rescan();
    void close();
    void getCalendar(QString user_id);
    void getDevicecStatus();

public slots:
  void connected(QString type);
  void disconnected(QString type);
  void scanResult(QString id, QString type);
  QString getServerUrl();

signals:
  void deviceStatusChanged(QStringList type);
  void scanSuccess(QString name, QString time, QString type, QString userid);
  void scanFail(QString error);
  void updateStatusBar(QString text, QString rgb);
  void receivedCalendar(QJsonArray jsonArray);
  void receivedCalendarFail(QString error);
  void receivedSetting(QJsonObject jsonObj);
  void receivedSettingFail(QString error);

private:
    void connectDevices();
    void connectServer();
    void updateDeviceStatus();

    int m_status; // 0: active, 1: not registered, 2: not exist license , 3: server error
    ApiHandle* m_apiHandle;
    QList<IScanDevice*> m_scanDevices;
    QString m_license_path = "/home/pi/Desktop/License";
    QString m_url_server = "";
    QString m_device_token = "";
    QString m_scanPath = "";
};

#endif // MACHINE_H
