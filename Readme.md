# Requirements
You will **need** the following **software**:
* The official Arduino IDE -> [GitHub](https://www.arduino.cc/en/Main/Software)
    * Temperature Control Library -> [GitHub](https://github.com/milesburton/Arduino-Temperature-Control-Library)
    * One Wire library -> [GitHub](https://github.com/PaulStoffregen/OneWire)
* Python 3.6 or higher -> [Python.org](https://www.python.org/downloads/)
    * Pyserial library -> [PythonHosted.org](https://pythonhosted.org/pyserial/pyserial.html#installation))

# Get it running
## The Nano
* Connect your Arduino Nano to the PC via usb cable
* Find the *COM* port the Nano is connected to
* Upload the *sobomo.ino* script to the nano

## The Logger
**Currently broken!** Do not use unless you can fix.

# The SERIAL syntax
A typical line communicated over the serial port is set to look as follows:
```C++
'#'                 // just a character showing start of a data line.
int readCount,      // reading count
float tankTemp,     // Tank temperature in Celcius
float pipeTemp,     // Pipe heater temperature in Celcius
int pumpState,      // Pump status - 1 for on, 0 for off
int warningState    // Freeze warning - 1 for on, 0 for off
```

# Acknowledgements
The address_finder_i2c.ino script was written by Shalvan from [Hacktronics](https://www.hacktronics.com/Tutorials/arduino-1-wire-address-finder.html).
