// This sketch looks for 1-wire devices and prints their addresses (serial number)
// to the UART, in a format that is useful in Arduino sketches.

// --- Include Libraries
// OneWire, found at http://www.pjrc.com/teensy/td_libs_OneWire.html
#include <OneWire.h>

// --- Configure variables
int ONE_WIRE_PIN = 2;
OneWire wire(ONE_WIRE_PIN);

// --- Main Functions
void setup(void) {
  Serial.begin(9600);
  discoverOneWireDevices();
}

void loop(void) { }

// --- Custom Functions
void discoverOneWireDevices(void) {
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
   
  Serial.print("Looking for I2C devices on pin ");
  Serial.println(ONE_WIRE_PIN);
  
  while(wire.search(addr)) {
    // Print the address
    Serial.print("Device found at address [");
    for( i = 0; i < 8; i++) {
      // -0x- XX
      Serial.print("0x");
      // 0x -0 or X- X
      if (addr[i] < 16) {
        Serial.print('0');
      }
      // 0x -XX-
      Serial.print(addr[i], HEX);
      // Comma seperators
      if (i < 7) {
        Serial.print(", ");
      }
    }

    Serial.print("] CRC [");
    
    // Check CRC
    if ( OneWire::crc8( addr, 7) != addr[7]) {
        Serial.print("INVALID");
    } else {
        Serial.print(OneWire::crc8( addr, 7));
    }
    Serial.println("]");
  }
  wire.reset_search();
  return;
}
