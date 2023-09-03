/*
File Name:  N24C32.cpp
Author:     Jack Orton


Description:

A I2C wrapper allowing use of the N24C32 EEPROM in arduino framework using the Wire library
*/

#include "N24C32.h"
TwoWire Wire2(PB7, PA15); //TODO remove this from here ideally

//default A2, A1 and A0 to 0, giving an address of 0x50
int N24C32_address = 0x50;

void N24C32_wire_config(uint32_t scl, uint32_t sda, uint32_t freq_khz)
{
    //configure I2C for memory interface
    Wire2.flush();
    Wire2.setSCL(scl);
    Wire2.setSDA(sda);
    //Wire2.setClock(freq_khz*1000);
    Wire2.begin();
}

void N24C32_wire_config(uint32_t scl, uint32_t sda, uint32_t freq_khz, int hex_address)
{
    //configure I2C for memory interface
    Wire2.setSCL(scl);
    Wire2.setSDA(sda);
    Wire2.setClock(freq_khz*1000);
    Wire2.begin();

    N24C32_address = hex_address;
}


byte N24C32_convert_U16_high_order(uint16_t u16_val)
{
    return u16_val & 0xff;
}
byte N24C32_convert_U16_low_order(uint16_t u16_val)
{
    return (u16_val >> 8) & 0xff;
}

int N24C32_byte_write(byte address_1, byte address_2, byte data)
{
    Wire2.beginTransmission(N24C32_address);
    Wire2.write(address_1);
    Wire2.write(address_2);
    Wire2.write(data);
    Serial2.print("end send: ");
    Serial2.println(Wire2.endTransmission());

    return 1;

}

int N24C32_page_write(byte address_1, byte address_2, byte *data, int num_bytes)
{
    if (num_bytes > 32)
    {
        return 0;
    }

    Wire2.beginTransmission(N24C32_address);
    Wire2.write(address_1);
    Wire2.write(address_2);
    for (int i = 0; i < num_bytes; i++)
    {
        Wire2.write(data[i]);
    }
    
    Wire2.endTransmission();

    return 1;
}

int N24C32_immediate_read(byte *data, uint16_t num_bytes)
{
    int num_bytes_received = Wire2.requestFrom(N24C32_address, num_bytes);

    if (num_bytes_received > num_bytes)
    {
        return 0;
    }

    for (int i = 0; i < num_bytes_received; i++)
    {
        data[i] = Wire2.read();
    }

    return 1;
}

int N24C32_selective_read(byte address_1, byte address_2, byte *data, uint16_t num_bytes)
{
    Wire2.beginTransmission(N24C32_address);
    Wire2.write(address_1);
    Wire2.write(address_2);
    Wire2.endTransmission();

    int num_bytes_received = Wire2.requestFrom(N24C32_address, num_bytes);
    Serial2.print("     bytes received: ");
    Serial2.print(num_bytes_received);
    if (num_bytes_received > num_bytes)
    {
        return 0;
    }

    for (int i = 0; i < num_bytes_received; i++)
    {
        data[i] = Wire2.read();
    }

    return 1;
}