/**
 * This sketch monitors two or more temperature sensors connected
 * together through i2c/1-wire to the Arduino at a certain pin,
 * and prints their temperatures to the serial stream after set intervals.
*/

// --- Include Libraries
// OneWire, found at https://github.com/PaulStoffregen/OneWire
//#include <OneWire.h>
// DallasTemperature, found at https://github.com/milesburton/Arduino-Temperature-Control-Library
#include <DallasTemperature.h>

// --- Configuration constants
// PIN 2 is Digital Pin 2 (see http://www.pighixxx.com/test/wp-content/uploads/2014/11/nano.png)
#define ONE_WIRE_BUS_PIN 2
// Read intervals - keep above 1000
#define SENSOR_READ_INTERVAL 5000 // 1000=1 second
// LED blink duration
#define LED_BLINK_DURATION 1000 // 1 second
// Sensor resolution has to do with accuracy but slows measure rates. 10 is recommended.
#define SENSOR_RESOLUTION 10
// Serial connection bit-rate.
#define SERIAL_BAUDRATE 9600
// The limits used for controlling the pump, in relation to the tank temperature.
#define PUMP_LOWER_LIMIT 4.0
#define PUMP_UPPER_LIMIT 8.0
#define PUMP_OUTPUT_PIN 4
// The limits for freezing warning.
#define FREEZE_LOWER_TEMP 4.5
#define FREEZE_UPPER_TEMP 8.0
#define WARNING_OUTPUT_PIN 5
// Status constants
#define STATUS_ON 1
#define STATUS_OFF 0

// Pass our oneWire reference to Dallas Temperature.
OneWire oneWire(ONE_WIRE_BUS_PIN);
DallasTemperature sensors(&oneWire);

// --- Configure devices here. To find adresses for connected sensors, use the i2c address finder script.
// The water tank sensor.
DeviceAddress tank_sensor = {0x28, 0xFF, 0xB3, 0x3D, 0xC2, 0x16, 0x04, 0xA7};
// The heating pipes sensor.
DeviceAddress pipe_sensor = {0x28, 0xFF, 0xF1, 0x57, 0xC2, 0x16, 0x04, 0x46};

// --- Counters and statuses
// Reading counter
int readingCounter = 1;
// Status of the pump and warning pins
int currentPumpStatus = STATUS_OFF;
int currentWarningStatus = STATUS_OFF;

// --------------------------------- Custom Functions --------------------------------

/**
 * Blink the LED for a configured ammount of time.
*/
void blinkLED(void)
{
  // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_BUILTIN, HIGH);
  // wait for a second
  delay(LED_BLINK_DURATION);
  // turn the LED off by making the voltage LOW
  digitalWrite(LED_BUILTIN, LOW);
}

/**
 * Do a check on the temperatures to see if the pump's status must change.
 * 1 means pump should be on, 0 means it should be off.
*/
void doPumpControlCheck(float tankTemp, float pipeTemp)
{
  // Configure the limits the pump functions around.
  float lowerLimit = tankTemp + PUMP_LOWER_LIMIT;
  float upperLimit = tankTemp + PUMP_UPPER_LIMIT;

  if (pipeTemp <= lowerLimit)
  {
    // If pump is turned on, check if it should turn off.
    if (currentPumpStatus == STATUS_ON)
    {
      togglePumpStatus();
    }
  }
  else if (pipeTemp > upperLimit)
  {
    // For the pump to turn on, it has to be above the minimum pump temperature.
    if (currentPumpStatus == STATUS_OFF)
    {
      togglePumpStatus();
    }
  }
}

/**
 * Toggle the pump's current state
 */
void togglePumpStatus()
{
  if (currentPumpStatus == STATUS_ON)
  {
    // Pump is on, turn off
    Serial.println("event: pump off");
    digitalWrite(PUMP_OUTPUT_PIN, LOW);
    currentPumpStatus = STATUS_OFF;
  }
  else
  {
    // Pump is off, turn on
    Serial.println("event: pump on");
    digitalWrite(PUMP_OUTPUT_PIN, HIGH);
    currentPumpStatus = STATUS_ON;
  }
}

void doWarningCheck(float tankTemp, float pipeTemp)
{
  // Check tank temperature
  if (tankTemp <= FREEZE_LOWER_TEMP)
  {
    // Below or on minimum, turn on warning
    if (currentWarningStatus == STATUS_OFF)
    {
      toggleWarningStatus();
    }
    // and stop the pump
    if (currentPumpStatus == STATUS_ON)
    {
      togglePumpStatus();
    }
  }
  else if (tankTemp > FREEZE_UPPER_TEMP)
  {
    // Turn off the warning since temperature is above upper limit
    if (currentWarningStatus == STATUS_ON)
    {
      toggleWarningStatus();
    }
    // No need to worry about pump since it gets checked after warning check in the loop function
  }
}

void toggleWarningStatus()
{
  if (currentWarningStatus == STATUS_ON)
  {
    // Warning light is on, turn off
    Serial.println("event: warning off");
    digitalWrite(WARNING_OUTPUT_PIN, LOW);
    currentWarningStatus = STATUS_OFF;
  }
  else
  {
    // Warning light is off, turn on
    Serial.println("event: warning on");
    digitalWrite(WARNING_OUTPUT_PIN, HIGH);
    currentWarningStatus = STATUS_ON;
  }
}

// ---------------------------------- Core Functions ----------------------------------
//
void setup()
{
  // Configure LED
  pinMode(LED_BUILTIN, OUTPUT);
  // Configure PUMP pin
  pinMode(PUMP_OUTPUT_PIN, OUTPUT);
  // Configure WARNING pin
  pinMode(WARNING_OUTPUT_PIN, OUTPUT);

  // Start serial port
  Serial.begin(SERIAL_BAUDRATE);
  Serial.print("Temperature Control Library Version ");
  Serial.println(DALLASTEMPLIBVERSION);

  // Initialize the Temperature measurement library
  sensors.begin();

  // Set the reading resolution (Can be 9 to 12 bits .. lower is faster)
  sensors.setResolution(tank_sensor, SENSOR_RESOLUTION);
  sensors.setResolution(pipe_sensor, SENSOR_RESOLUTION);

  // Open serial port
  Serial.print("Reading intervals set to ");
  Serial.print((SENSOR_READ_INTERVAL / 1000));
  Serial.println(" seconds.");
}

//
void loop()
{
  // Flash builtin LED
  blinkLED();

  // Call devices on the bus to read temperatures
  sensors.requestTemperatures();
  // Capture temperatures
  float tankTemperature = sensors.getTempC(tank_sensor);
  float pipeTemperature = sensors.getTempC(pipe_sensor);

  // Do warning check
  doWarningCheck(tankTemperature, pipeTemperature);

  // Do pump check, if warning light is off
  if (currentWarningStatus == STATUS_OFF)
  {
    doPumpControlCheck(tankTemperature, pipeTemperature);
  }

  // Print latest reading
  Serial.print("#");            // Start of line
  Serial.print(readingCounter); // Reading count
  Serial.print(",");
  Serial.print(tankTemperature); // Tank sensor
  Serial.print(",");
  Serial.print(pipeTemperature); // Pipe sensor
  Serial.print(",");
  Serial.print(currentPumpStatus); // Pump state
  Serial.print(",");
  Serial.print(currentWarningStatus); // Freeze state
  Serial.println("");          // End-of-line

  // Read every ten seconds (1 from blink + 9 from this)
  delay(SENSOR_READ_INTERVAL - LED_BLINK_DURATION);

  readingCounter = readingCounter + 1;
}
