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
    Wire.beginTransmission(N24C32_address);
    Wire.write(address_1);
    Wire.write(address_2);
    Wire.write(data);
    Wire.endTransmission();

}

int N24C32_page_write(byte address_1, byte address_2, byte *data, int num_bytes)
{
    if (num_bytes > 32)
    {
        return 0;
    }

    Wire.beginTransmission(N24C32_address);
    Wire.write(address_1);
    Wire.write(address_2);
    for (int i = 0; i < num_bytes; i++)
    {
        Wire.write(data[i]);
    }
    
    Wire.endTransmission();
}

int N24C32_immediate_read(byte *data, uint16_t num_bytes)
{
    int num_bytes_received = Wire.requestFrom(N24C32_address, num_bytes, true);

    if (num_bytes_received > num_bytes)
    {
        return 0;
    }

    for (int i = 0; i < num_bytes_received; i++)
    {
        data[i] = Wire.read();
    }
}

int N24C32_selective_read(byte address_1, byte address_2, byte *data, uint16_t num_bytes)
{
    Wire.beginTransmission(N24C32_address);
    Wire.write(address_1);
    Wire.write(address_2);
    Wire.endTransmission();

    int num_bytes_received = Wire.requestFrom(N24C32_address, num_bytes, true);

    if (num_bytes_received > num_bytes)
    {
        return 0;
    }

    for (int i = 0; i < num_bytes_received; i++)
    {
        data[i] = Wire.read();
    }
}