/*
File Name:  N24C32.h
Author:     Jack Orton


Description:

A I2C wrapper allowing use of the N24C32 EEPROM in arduino framework using the Wire library
*/

//ifndef to prevent multiple copies of this being compiled
#ifndef N24C32_h
#define N24C32_h


#include <Arduino.h>
#include <Wire.h>
#include "hardware_functions.h"


//address of device.
//N24C32 takes an 8 bit address
//format is [1][0][1][0][A2][A1][A0][R/W]
//[r/w] will be handled by the wire library, so we just need the 7 bit address

//default A2, A1 and A0 to 0, giving an address of 0x50
int N24C32_address = 0x50;

void N24C32_wire_config(uint32_t scl, uint32_t sda);
void N24C32_wire_config(uint32_t scl, uint32_t sda, int hex_address);


/*
Byte Write
To write data to memory, the Master creates a START
condition on the bus and then broadcasts a Slave address
with the R/W bit set to ‘0’. The Master then sends two
address bytes and a data byte and concludes the session by
creating a STOP condition on the bus. The Slave responds
with ACK after every byte sent by the Master (Figure 6). The
STOP starts the internal Write cycle, and while this
operation is in progress (tWR), the SDA output is tri-stated
and the Slave does not acknowledge the Master (Figure 7).
*/
int N24C32_byte_write(byte address_1, byte address_2, byte data);


/*
Page Write
The Byte Write operation can be expanded to Page Write,
by sending more than one data byte to the Slave before
issuing the STOP condition (Figure 8). Up to 32 distinct data
bytes can be loaded into the internal Page Write Buffer
starting at the address provided by the Master. The page
address is latched, and as long as the Master keeps sending
data, the internal byte address is incremented up to the end
of page, where it then wraps around (within the page). New
data can therefore replace data loaded earlier. Following the
STOP, data loaded during the Page Write session will be
written to memory in a single internal Write cycle (tWR).
*/
int N24C32_page_write(byte address_1, byte address_2, byte *data, int num_bytes);



/*
Sequential Read
If, after receiving data sent by the Slave, the Master
responds with ACK, then the Slave will continue
transmitting until the Master responds with NoACK
followed by STOP (Figure 12). During Sequential Read the
internal byte address is automatically incremented up to the
end of memory, where it then wraps around to the beginning
of memory
*/

/*
Immediate Read
To read data from memory, the Master creates a START
condition on the bus and then broadcasts a Slave address
with the R/W bit set to ‘1’. The Slave responds with ACK
and starts shifting out data residing at the current address.
After receiving the data, the Master responds with NoACK
and terminates the session by creating a STOP condition on
the bus (Figure 10). The Slave then returns to Standby mode.
*/
int N24C32_immediate_read(byte *data, uint16_t num_bytes);

/*
Selective Read
To read data residing at a specific address, the selected
address must first be loaded into the internal address register.
This is done by starting a Byte Write sequence, whereby the
Master creates a START condition, then broadcasts a Slave
address with the R/W bit set to ‘0’ and then sends two
address bytes to the Slave. Rather than completing the Byte
Write sequence by sending data, the Master then creates a
START condition and broadcasts a Slave address with the
R/W bit set to ‘1’. The Slave responds with ACK after every
byte sent by the Master and then sends out data residing at
the selected address. After receiving the data, the Master
responds with NoACK and then terminates the session by
creating a STOP condition on the bus (Figure 11).
*/
int N24C32_selective_read(byte address_1, byte address_2, byte *data, uint16_t num_bytes);





#endif