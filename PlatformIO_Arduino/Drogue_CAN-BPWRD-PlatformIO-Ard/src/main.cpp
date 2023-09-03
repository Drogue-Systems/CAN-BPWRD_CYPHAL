#include <Arduino.h>
#include "hardware_functions.h"
#include "serial_display.h"


void setup() {

  hardware_setup(); //configure hardware (GPIO, UART, CAN, I2C etc)

}

void loop() {

  hardware_service(); //perform periodic hardware tasks (ie reading bus voltages)



  //comment display_service out if not wanted, add considerable resource usage!
  display_service();

}
