/*
File Name:  N24C32.cpp
Author:     Jack Orton


Description:

A I2C wrapper allowing use of the N24C32 EEPROM in arduino framework using the Wire library
*/

#include "N24C32.h"

void N24C32_wire_config(uint32_t scl, uint32_t sda)
{
    //configure I2C for memory interface
    Wire.setSCL(scl);
    Wire.setSDA(sda);
    Wire.begin();
}

void N24C32_wire_config(uint32_t scl, uint32_t sda, int hex_address)
{
    //configure I2C for memory interface
    Wire.setSCL(scl);
    Wire.setSDA(sda);
    Wire.begin();

    N24C32_address = hex_address;
}

int N24C32_byte_write(byte address_1, byte address_2, byte data)
{

}

int N24C32_page_write(byte address_1, byte address_2, byte data[32], int num_bytes)
{

}

int N24C32_immediate_read(byte *data, uint16_t num_bytes)
{

}

int N24C32_selective_read(byte address_1, byte address_2, byte *data, uint16_t num_bytes)
{
    
}