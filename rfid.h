#ifndef RFID_H
#define RFID_H
#include "iscandevice.h"

/*
 * README auto  setname for device
    https://unix.stackexchange.com/questions/66901/how-to-bind-usb-device-under-a-static-name
    ATTRS{idProduct}=="ea60"
    ATTRS{idVendor}=="10c4"
    nano /etc/udev/rules.d/99-usb-serial.rules
    SUBSYSTEM=="tty", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="ea60", SYMLINK+="tcg_rfid"

    /dev/tcg_rfid
*/

class RFID : public IScanDevice
{
    void run() override;
public:
    RFID();
    ~RFID();

    void setRescan() override;
    void closeport() override;
private:
   int fd =-1;

   void openport();

};

#endif // RFID_H
