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
* Edit log_settings.py. Change the COM port number to the one you are connected to the Nano with.
  * You can try to find the port via Device Manager -> Ports (COM & LPT)
* Make sure there's a folder called 'readings' in the same directory you are running the logger script in. 
  * If it's not there, create it.
  * The script needs to be able to write files inside it.
* Run the logger.py script from command prompt, or create a batch file.

The readings will be logged in csv files in the previously mentioned 'readings' folder, stamped according to your system's date.

# The SERIAL syntax
A typical line communicated over the serial port is set to look as follows:
```C++
int readCount,      // reading count
float tankTemp,     // Tank temperature in Celcius
float pipeTemp,     // Pipe heater temperature in Celcius
int pumpState,      // Pump status - 1 for on, 0 for off
int warningState    // Freeze warning - 1 for on, 0 for off
```
NOTE: Non-data or comment lines start with a '#'

# Acknowledgements
The address_finder_i2c.ino script was written by Shalvan from [Hacktronics](https://www.hacktronics.com/Tutorials/arduino-1-wire-address-finder.html).
