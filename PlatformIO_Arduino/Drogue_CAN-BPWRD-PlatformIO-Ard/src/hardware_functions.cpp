
/*
File Name:  hardware_functions.cpp
Author:     Jack Orton


Description:

This file acts as a higher level hardware abstraction layer, performaing most of the direct hardware interactions
with the board and providing an easy interface for the main code.
*/

#include <Arduino.h>
#include "hardware_functions.h"
#include <Wire.h>
#include "N24C32.h"



void hardware_setup()
{
    //begin serial on USART2 for display
    Serial2.begin(921600);

 
    //Config wire library for N24C32 chip
    //N24C32_wire_config(PA15, PB7, 100);

    

    //configure ADC pins for can bus voltage readings
    pinMode(PB14, INPUT);
    pinMode(PB15, INPUT);


}

void hardware_service()
{

    //############################################################
    //CAN Bus voltage measurements

    //static variable to track when the last measurments were taken
    static uint32_t last_voltage_input_measurement = 0;

    //if a new sample is due to be taken, then take new samples and reset the timer
    if (millis() - last_voltage_input_measurement > VOLTAGE_INPUT_MILLISECONDS_PER_SAMPLE)
    {
        //call functions to update the rolling average voltage measurement
        calculate_return_average_voltage_can(1, 0); //bus 1, no return needed
        calculate_return_average_voltage_can(2, 0); //bus 2, no return needed

        //update time of last measurement to right now
        last_voltage_input_measurement = millis();
    }

    //############################################################



    static uint32_t last_eeprom_test = 0;
    if ( (millis() - last_eeprom_test) > 100)
    {
        eeprom_test(1);
        last_eeprom_test = millis();
    }
}


float return_voltage_input_can(int bus)
{
    if (bus == 1)
    {
        return calculate_return_average_voltage_can(1, 1);
    }
    else if (bus == 2)
    {
        return calculate_return_average_voltage_can(2, 1);
    }
    else {
        return 252;
    }
}


void eeprom_test(int mode)
{
    static uint16_t read_pos = 0;
    static uint16_t write_pos = 10;
    static uint8_t write_val = 0;
    static uint8_t read_data = 0;

    static int RW_switch = 0;
    if (mode == 1)
    {
        if (RW_switch == 0)
        {
            N24C32_byte_write(N24C32_convert_U16_high_order(write_pos), N24C32_convert_U16_low_order(write_pos), write_val);
            write_pos++;
            write_val++;

            if (write_pos > 1000)
            {
                write_pos = 0;
            }
            if (write_val > UINT8_MAX)
            {
                write_val = 0;
            }
        }

        if (RW_switch == 1)
        {
            //N24C32_selective_read(N24C32_convert_U16_high_order(read_pos), N24C32_convert_U16_low_order(read_pos), &read_data, 1);
            N24C32_immediate_read(&read_data, 1);
            read_pos++;
            if (read_pos > 1000)
            {
                read_pos = 0;
            }
        }

        if (RW_switch == 0)
        {
            RW_switch = 1;
        }
        else
        {
            RW_switch = 0;
        }
    }

    else if (mode == 2)
    {
        Serial2.println("Write Pos   |   Write Val  |   Read pos   |   Read Val");
        Serial2.print(write_pos);
        Serial2.print("                 ");
        Serial2.print(write_val);
        Serial2.print("                 ");
        Serial2.print(read_pos);
        Serial2.print("                 ");
        Serial2.println(read_data);
    }
  
}

float calculate_return_average_voltage_can(int bus, int option)
{
    //initialise arrays to store the ADC readings of input votage
    static uint32_t bus_1_readings_array[VOLTAGE_INPUT_SAMPLES_COUNT] = {0};
    static uint32_t bus_2_readings_array[VOLTAGE_INPUT_SAMPLES_COUNT] = {0};


    //if option is 0 then we are updating the array of measurements, this code will run more
    //often than option 1, so we don't calculate average in here to cycles time taken
    if (option == 0)
    {
        //track current position in array
        static int8_t current_pos = 0;

        //read ADC value into array in current pos position
        bus_1_readings_array[current_pos] = analogRead(PIN_CAN_1_VOLTAGE);
        bus_2_readings_array[current_pos] = analogRead(PIN_CAN_2_VOLTAGE);

        //increment current pos
        current_pos++;

        //reset current pos to 0 if last array position reached
        if (current_pos > (VOLTAGE_INPUT_SAMPLES_COUNT-1))
        {
            current_pos = 0;
        }

        return 0;
    }

    //if option is 1, then we calculate the average and return it
    else if (option == 1)
    {
        //if voltage of bus 1 requested
        if (bus == 1)
        {
            //initialise a variable to 0 to calculate average with
            float bus_1_voltage = 0;

            //sum up array elements
            for (int sum_pos = 0; sum_pos<(VOLTAGE_INPUT_SAMPLES_COUNT); sum_pos++)
            {
                bus_1_voltage += bus_1_readings_array[sum_pos];
            }

            //average the summed array
            bus_1_voltage = bus_1_voltage   /   (float)VOLTAGE_INPUT_SAMPLES_COUNT;

            //convert ADC value into real voltage on MCU pin
            bus_1_voltage = (bus_1_voltage   /   ((float)1024) * (float)3.3);

            //Convert pin voltage to input voltage by applying the measured ratio of potential divider
            bus_1_voltage = bus_1_voltage   /   VOLTAGE_INPUT_MEASURED_RATIO_CAN_1;
            
            //return average value
            return bus_1_voltage;
        }

        //if voltage of bus 2 requested
        else if (bus == 2)
        {
            //initialise a variable to 0 to calculate average with
            float bus_2_voltage = 0;

            //sum up array elements
            for (int sum_pos = 0; sum_pos<(VOLTAGE_INPUT_SAMPLES_COUNT); sum_pos++)
            {
                bus_2_voltage += bus_2_readings_array[sum_pos];
            }

            //average the summed array
            bus_2_voltage = bus_2_voltage   /   (float)VOLTAGE_INPUT_SAMPLES_COUNT;

            //convert ADC value into real voltage on MCU pin
            bus_2_voltage = (bus_2_voltage   /   ((float)1024) * (float)3.3);

            //Convert pin voltage to input voltage by applying the measured ratio of potential divider
            bus_2_voltage = bus_2_voltage   /   VOLTAGE_INPUT_MEASURED_RATIO_CAN_2;
            
            //return average value
            return bus_2_voltage;
        }

        //if bus number wrong
        else
        {
            return 254;
        }
    }

    //if option wrong
    else
    {
        return 253;
    }
    
}

