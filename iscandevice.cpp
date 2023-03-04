#include "iscandevice.h"

void IScanDevice::setFail()
{
    m_mutex.lock();
    m_status = -1;
    m_mutex.unlock();
    emit disconnected(m_type);
}

void IScanDevice::setSuccess()
{
    m_mutex.lock();
    m_status = 1;
    m_mutex.unlock();
    emit connected(m_type);
}

bool IScanDevice::isOk()
{
    return m_status != -1;
}

QString IScanDevice::getType()
{
    return m_type;
}
