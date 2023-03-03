#include "rfid.h"
#include "qdebug.h"
#include "sl500.h"

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include <time.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#define BUZZ_SIZE 1024
#define FAST 10
#define SLOW 50

RFID::RFID()
{
    m_type = "RFID";
    openport();
}

RFID::~RFID()
{
    qDebug() << "close port...";
    close(fd);
}

void RFID::closeport()
{
    m_mutex2.lock();
    qDebug() << "close port...";
    if(fd != -1)
    {
        try {
            rf_light(fd, LED_RED);
            rf_init_com(fd, BAUD_19200);
        }  catch (std::exception &e) {

        }
        close(fd);
    }
    m_mutex2.unlock();
}

void RFID::openport()
{
    m_mutex2.lock();
    qDebug() << "open port...";
    uint8_t buf[100];
    /* Set up serial port */
    fd = open_port();
    if(fd != -1)
    {
        printf("\nSpeeding up communication to 115200 baud...\n");
        rf_init_com(fd, BAUD_115200);

        rf_get_model(fd, sizeof(buf), buf);
        printf("Model: %s\n", buf);
        rf_light(fd, LED_GREEN);
        rf_beep(fd, FAST);

        setSuccess();
    }
    else
    {
        qDebug() << "open port failed...";
       setFail();
    }
    m_mutex2.unlock();
}

void delay(int milliseconds)
{
    long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}

void inputSuccess(int fd)
{
        rf_beep(fd, FAST);
        rf_light(fd, LED_GREEN);
        delay(100);
        rf_light(fd, LED_OFF);
        delay(100);
        rf_beep(fd, FAST);
        rf_light(fd, LED_GREEN);
        delay(100);
        rf_light(fd, LED_OFF);
        delay(1000);
        rf_light(fd, LED_GREEN);

}
void RFID::run()
{

    qDebug() << "loop rfid...";
    while(1)
    {
        if(fd!=-1)
        {
            // new start
            m_mutex.lock();
            if(m_status != -1) // continue waiting
            {
                m_mutex.unlock();
                uint8_t status;
                unsigned int card_no;
                try {
                    m_mutex2.lock();
                    status = rf_request(fd);
                    status = rf_anticoll(fd, &card_no);

                    if (status != 0)
                    {
                        //setFail();
                    }
                    else{
                        qDebug() << card_no << "\n";
                        printf("Card number: %u (0x%08x)\n", card_no, card_no);
                        emit scanResult(QString::number(card_no), m_type);
                        inputSuccess(fd);
                    }
                }  catch (std::exception& e) {
                    setFail();
                }

                m_mutex2.unlock();
            }
        }
        delay(500);
    }
}

void RFID::setRescan()
{
    closeport();
    openport();
}

