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
    void stopScan();
    void rescan();

    void isScanning();

public slots:
  void connected(QString type);
  void disconnected(QString type);
  void scanResult(QString id, QString type);

signals:
  void deviceStatusChanged(QString type);
  void scanStatusChanged(QString id, QString type);

private:
    void callBack();
    void connectDevices();

    int m_status;
    ApiHandle* m_apiHandle;
    QList<IScanDevice*> m_scanDevices;
};

#endif // MACHINE_H
