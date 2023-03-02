#include "machine.h"
#include "rfid.h"
#include "qdebug.h"

Machine::Machine(QObject* parent) : QObject(parent)
{
    IScanDevice* rfid = new RFID();
    m_scanDevices.append(rfid);
    connectDevices();
    startScan();
}

void Machine::connectDevices()
{
    for(auto scanDevice :  m_scanDevices)
    {
        connect(scanDevice, &IScanDevice::connected, this, &Machine::connected);
        connect(scanDevice, &IScanDevice::disconnected, this, &Machine::disconnected);
        connect(scanDevice, &IScanDevice::scanResult, this, &Machine::scanResult);
    }
}

Machine::~Machine()
{

}

void Machine::close()
{
    for(auto scanDevice :  m_scanDevices)
    {
        scanDevice->closeport();
    }
}

void Machine::startScan()
{
    for(auto scanDevice :  m_scanDevices)
    {
        scanDevice->start();
    }
}

void Machine::stopScan()
{

}

void Machine::rescan()
{
    for(auto scanDevice :  m_scanDevices)
    {
        scanDevice->setRescan();
    }
}

void Machine::connected(QString type)
{
    qDebug() << "connected: " + type;
    emit deviceStatusChanged(type);
}
void Machine::disconnected(QString type)
{
    qDebug() << "disconnected: " + type;
    emit deviceStatusChanged(type);
}

void Machine::scanResult(QString id, QString type)
{
    qDebug() << "scanResult: " + id + " - " + type;
    emit scanStatusChanged(id, type);
}
