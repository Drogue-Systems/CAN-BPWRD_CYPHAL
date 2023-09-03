#include <Arduino.h>
#include "hardware_functions.h"
#include "serial_display.h"
#include "N24C32.h"

#include <Wire.h>

void setup() {

  hardware_setup(); //configure hardware (GPIO, UART, CAN, I2C etc)

  // Wire.setSCL(PA_15);
  // Wire.setSDA(PB_7);
  // Wire.setTimeout(100000000);
  // Wire.begin();

  // delay(100);

  // Wire.beginTransmission(0x50);
  // Wire.write(0x01);
  // Wire.write(0x01);
  // //Wire.write(0x5F);
  // Serial2.print(Wire.endTransmission(true));
  // Wire.end();
  // Wire.flush();

  // delay(100);

  

}

void loop() {

  hardware_service(); //perform periodic hardware tasks (ie reading bus voltages)



  //comment display_service out if not wanted, add considerable resource usage!
  display_service();

}
