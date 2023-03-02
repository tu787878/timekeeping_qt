#ifndef ISCANDEVICE_H
#define ISCANDEVICE_H
#include <QThread>
#include <QMutex>

class IScanDevice : public QThread
{
    Q_OBJECT

public:
    virtual void setRescan() = 0;
    virtual void closeport() = 0;

signals:
    void connected(QString type);
    void disconnected(QString type);
    void scanResult(QString id, QString type);

protected:
    void setFail();
    void setSuccess();

    int m_status = 0; // 0: new, 1: rescan, -1: fail, ...:success
    QMutex m_mutex;
    QString m_type;
};

#endif // ISCANDEVICE_H
