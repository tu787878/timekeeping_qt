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
}

void RFID::openport()
{
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

        m_mutex.lock();
        m_status = 2;
        m_mutex.unlock();
        emit connected(m_type);
    }
    else
    {
        qDebug() << "open port failed...";
        m_mutex.lock();
        m_status = -1;
        m_mutex.unlock();
        emit disconnected(m_type);
    }
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
                //qDebug() << "check card...";
                // TODO: set m_status = 2 if success or -1 if fail

            }
        }
    }
}

void RFID::setRescan()
{
    closeport();
    openport();
}

