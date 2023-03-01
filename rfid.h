#ifndef RFID_H
#define RFID_H
#include "iscandevice.h"

class RFID : public IScanDevice
{
    void run() override;
public:
    RFID();
    ~RFID();

    void setRescan() override;
};

#endif // RFID_H
