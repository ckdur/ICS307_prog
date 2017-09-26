#!/bin/bash
gcc main.c ICS307_spi.c -I./ -o ICS307_spi -lftd2xx -lMPSSE -lm
sudo rmmod ftdi_sio
sudo rmmod usbserial
sudo ./ICS307_spi

