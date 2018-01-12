'''Requirements
You will need the following software:
> The official Arduino IDE (https://www.arduino.cc/en/Main/Software)
    > Temperature Control Library (https://github.com/milesburton/Arduino-Temperature-Control-Library)
> Python 3.6 or higher (https://www.python.org/downloads/)
    > Pyserial library (pip install pyserial in Command-line, or https://pythonhosted.org/pyserial/pyserial.html#installation) 
> A text-editor if you want to make changes.

No longer needed:
> Arduino libraries
    > One Wire library (https://github.com/PaulStoffregen/OneWire)

'''Get it running
''The Nano
> Connect your Arduino Nano to the PC via usb cable
> Upload the monitor script to the nano

''The Logger
> Check the settings.py file to configure the correct COM port and such.
    > Can be found by checking under ports in the Device Manager
> Save settings.py and run logger.py

'''The SERIAL syntax
A typical line communicated over the serial port is set to look as follows:
> \#                    -> just a character showing start of a data line.
> int32,                -> reading count
> float (2 decimals),   -> Tank temperature in Celcius
> float (2 decimals),   -> Pipe heater temperature in Celcius
> int32,                -> Pump status - 1 for on, 0 for off
> int32                 -> Freeze warning - 1 for on, 0 for off