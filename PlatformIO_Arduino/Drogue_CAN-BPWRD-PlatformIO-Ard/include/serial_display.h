/*
File Name:  serial_display.h
Author:     Jack Orton


Description:

Provides function interface for generating the serial display on USART2 (PA2)
*/
//ifndef to prevent multiple copies of this being compiled
#ifndef serial_display_h
#define serial_display_h

#define SERIAL_DISPLAY_REFRESH_HZ 8

void display_service();

#endif