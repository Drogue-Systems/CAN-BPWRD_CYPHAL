/*
File Name:  hardware_functions.h
Author:     Jack Orton


Description:

This file provides a function interface for other files to mkae use of the higher level hardware abstraction layer defined in
hardware_functions.cpp
*/

//ifndef to prevent multiple copies of this being compiled
#ifndef hardware_functions_h
#define hardware_functions_h

//define pins for voltage readings
#define PIN_CAN_1_VOLTAGE PB14
#define PIN_CAN_2_VOLTAGE PB15

//defines to configure voltage measurments on CAN bus voltages
#define VOLTAGE_INPUT_SAMPLES_COUNT 10 //keep a rolling average of X samples
#define VOLTAGE_INPUT_MILLISECONDS_PER_SAMPLE 200 //take a sample every X milliseconds

//define measured ratio of potential deividers on baord
#define VOLTAGE_INPUT_MEASURED_RATIO_CAN_1 0.075
#define VOLTAGE_INPUT_MEASURED_RATIO_CAN_2 0.075




//#########################################################
//"public" API
//These functions can be called from any part of the program to interface with board hardware
//---------------------------------------------------------

void hardware_setup();

//Call this repeatedly in the main loop to service any periodic actions required.
void hardware_service();

//return the current voltage input - this is the average over the last x seconds
float return_voltage_input_can(int bus);


//#########################################################




//#########################################################
//"private" functions
//Don't call these
//---------------------------------------------------------

//called by hardware_service() with option=0 to keep track of rolling average, 
//called by return_voltage_input_can_1() with option=1 to return average
float calculate_return_average_voltage_can(int bus, int option);









#endif //hardware_functions_h