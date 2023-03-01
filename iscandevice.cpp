#include "iscandevice.h"

void IScanDevice::setFail()
{
    m_status = -1;
    emit disconnected(m_type);
}

void IScanDevice::setSuccess()
{
    m_status = 2;
    emit connected(m_type);
}
