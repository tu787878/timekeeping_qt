#ifndef MACHINE_H
#define MACHINE_H

#include "iscandevice.h"
#include "apihandle.h"
#include <QObject>

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

public slots:
  void connected(QString type);
  void disconnected(QString type);
  void scanResult(QString id, QString type);

signals:
  void deviceStatusChanged(QString type);
  void scanSuccess(QString name, QString user_id, QString time);
  void scanFail(QString error);
  void updateStatusBar(QString text, QString rgb);
  void receivedCalendar();

private:
    void connectDevices();
    void connectServer();

    int m_status; // 0: active, 1: not registered, 2: not exist license , 3: server error
    ApiHandle* m_apiHandle;
    QList<IScanDevice*> m_scanDevices;
    QString m_license_path = "/home/pi/Desktop/License";
    QString m_url_server = "";
    QString m_device_token = "";
};

#endif // MACHINE_H
