#include "rfid.h"
#include "qdebug.h"

RFID::RFID()
{
    m_type = "RFID";
    qDebug() << "open port...";
    emit connected("RFID");
    // TODO: set m_status = 2 if success or -1 if fail
}
RFID::~RFID()
{
    qDebug() << "close port...";
}
void RFID::run()
{
    qDebug() << "loop rfid...";
    while(1)
    {
        // new start
        m_mutex.lock();
        if(m_status == 1) // restart
        {
            m_mutex.unlock();
            qDebug() << "close port...";
            qDebug() << "open port...";
            // TODO: set m_status = 2 if success or -1 if fail
            m_status = 2;
            emit connected("RFID");

        }
        else if(m_status != -1) // continue waiting
        {
            m_mutex.unlock();
            //qDebug() << "check card...";
            // TODO: set m_status = 2 if success or -1 if fail

        }
    }
}

void RFID::setRescan()
{
    m_mutex.lock();
    qDebug() << "rescan...";
    m_status = 1;
    m_mutex.unlock();

}

