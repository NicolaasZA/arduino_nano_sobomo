//
// This sketch monitors two or more temperature sensors connected
// together through i2c/1-wire to the Arduino at a certain pin,
// and prints their temperatures to the serial stream at intervals.
//

// --- Include Libraries
// OneWire, found at http://www.pjrc.com/teensy/td_libs_OneWire.html
#include <OneWire.h>
// DallasTemperature, found at https://github.com/milesburton/Arduino-Temperature-Control-Library
#include <DallasTemperature.h>

// --- Configure variables
#define ONE_WIRE_BUS_PIN 2
#define READ_TIMING 10000 // 10 seconds
#define SENSOR_RESOLUTION 10
#define MAX_SENSOR_COUNT 10

// Pass our oneWire reference to Dallas Temperature.
OneWire oneWire(ONE_WIRE_BUS_PIN);
DallasTemperature sensors(&oneWire);

// Sensor addresses, upto 10. Find it by using the address finder sketch.
byte deviceCount;
byte deviceAddrs[MAX_SENSOR_COUNT][8];

//DeviceAddress Probe_01 = { 0x28, 0xFF, 0xB3, 0x3D, 0xC2, 0x16, 0x04, 0xA7 };  // external temperature probe 
//DeviceAddress Probe_02 = { 0x28, 0xFF, 0xF1, 0x57, 0xC2, 0x16, 0x04, 0x46 };  // internal temperature probe

// Reading count
int readcount = 1;

// --- Main Functions
void setup()
{
  // pinmode for check led
  pinMode (LED_BUILTIN, OUTPUT);

  // start serial port to show results
  Serial.begin(9600);
  Serial.print("Temperature Control Library Version ");
  Serial.println(DALLASTEMPLIBVERSION);

  // Initialize the Temperature measurement library
  sensors.begin();

  // Count the devices
  deviceCount = sensors.getDeviceCount();
  if (deviceCount > MAX_SENSOR_COUNT) {
    deviceCount = MAX_SENSOR_COUNT;
  }

  // Prepare the devices
  Serial.println("Devices used:");
  oneWire.reset_search();
  for (byte i=0; i < deviceCount; i++) {
    // Grab its address
    oneWire.search(deviceAddrs[i]); 
    // Set the reading resolution (Can be 9 to 12 bits .. lower is faster)
    sensors.setResolution(deviceAddrs[i], SENSOR_RESOLUTION);
    // Print it
    Serial.print("&\t");
    for (byte j=0;j<8;j++) {
      Serial.print(deviceAddrs[i][j], HEX);
      // Seperators
      if (j<7) { Serial.print(":"); }
    }
    Serial.println();
  }  
 
  // print some serial monitor info
  Serial.println(deviceCount);
  Serial.print("Reading intervals set to ");
  Serial.print((READ_TIMING / 1000));
  Serial.println(" seconds.");
  Serial.println();
}

// --- Custom Functions
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC == -127.00) {
    Serial.print("0.0");
  }
  else {
    Serial.print(tempC, 1);
  }
}

void blinkLED(void) 
{
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
}

// -- Loop
void loop()
{
  // Flash builtin LED
  blinkLED();

  // Call devices on the bus to read temperature
  sensors.requestTemperatures();

  // Print latest reading
  Serial.print("$");
  Serial.print(readcount); // Reading count
  Serial.print(",");
  Serial.print("@"); // Timestamp placeholder
  Serial.print(",");
  // Readings
  for (int i=0; i < deviceCount; i++) {
    printTemperature(deviceAddrs[i]); 
    if (i < deviceCount - 1) {
      Serial.print(",");
    }
  }
  // End of line
  Serial.println("");
  
  // Read every ten seconds (1 from blink + 9 from this)
  delay(READ_TIMING - 1000);
  
  //Increment timecount
  readcount = readcount + 1;
}

