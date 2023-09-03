/*
File Name:  serial_display.cpp
Author:     Jack Orton


Description:

Provides function interface for generating the serial display on USART2 (PA2)
*/
#include <Arduino.h>
#include "serial_display.h"
#include "hardware_functions.h"



void display_service()
{
    static uint32_t last_refresh = 0;
    static float floated_FPS = 0;
    static uint16_t frame_count = 0;


    if(millis() - last_refresh > (1000/SERIAL_DISPLAY_REFRESH_HZ))
    {

        //wipe screen every now and then to remove any artifacts that arent overwritten
        if (frame_count > (SERIAL_DISPLAY_REFRESH_HZ*7))
        {
            Serial2.write(27);       // ESC command
            Serial2.print("[2J");    // clear screen command
            frame_count = 0;
        }

        Serial2.write("\x1B[H");


        

        Serial2.println("  _____                                         _____            _           _ ");
        Serial2.println(" |  __ \\                                       / ____|          | |         | |");
        Serial2.println(" | |  | |_ __ ___  _   _  __ _  ___   ______  | |    _   _ _ __ | |__   __ _| |");
        Serial2.println(" | |  | | '__/ _ \\| | | |/ _` |/ _ \\ |______| | |   | | | | '_ \\| '_ \\ / _` | |");
        Serial2.println(" | |__| | | | (_) | |_| | (_| |  __/          | |___| |_| | |_) | | | | (_| | |");
        Serial2.println(" |_____/|_|  \\___/ \\__,_|\\__, |\\___|           \\_____\\__, | .__/|_| |_|\\__,_|_|");
        Serial2.println("                          __/ |                       __/ | |                  ");
        Serial2.print("                         |___/                       |___/|_|      ");
        Serial2.print("FPS: ");
        Serial2.println(floated_FPS);
        Serial2.println("---------------------------------------------------------------------------------\n\n");
        Serial2.println("   CAN 1    |    CAN 2\n");
        Serial2.print("   ");
        Serial2.print(return_voltage_input_can(1));
        Serial2.print("V    |    ");
        Serial2.print(return_voltage_input_can(2));
        Serial2.println("V");





        uint32_t actual_frame_time = millis() - last_refresh;
        floated_FPS = 1000/(float)actual_frame_time;

        last_refresh = millis();
        frame_count++;
    }


}