##
# 
# by Louw Pretorius
##

import serial
import time
import datetime

import log_settings as settings


class BOL:
    EVENT = '@'
    INFORMATION = '#'


def makeFile():
    filename = 'temp_{:%Y%m%d_%H%M%S}'.format(datetime.datetime.now())
    mfile = open('readings//{}.csv'.format(filename), 'w')
    return mfile


if __name__ == "__main__":
    #### Configure serial
    ser = serial.Serial(settings.SERIAL_PORT_ID, settings.SERIAL_PORT_BAUDRATE)
    # Give stream moment to open
    time.sleep(0.2)

    ### Configure csv file
    datafile = makeFile()

    #### Information Display
    print('\nInput:\tSerial Port \'{}\''.format(ser.port))
    print('Output:\t{}'.format(datafile.name))
    print('Press Control + C to stop taking readings.\n')
    #### Read loop
    try:
        counter = 0
        deviceIndex = 1
        while ser.is_open:
            # read it
            text = str(ser.readline())

            # clean it
            text = text.replace('b', '').replace("\\n", '').replace("\\r", '')
            text = text.replace('\'', '').replace("\\t", '')
            text = text.strip()

            # check it
            if len(text) <= 0:
                continue
            
            # EVENT LINE
            if text[0] == BOL.EVENT:
                # Print
                print(text.replace(BOL.EVENT,'[EVENT]'))
                continue

            # EVENT LINE
            if text[0] == BOL.INFORMATION:
                # Print
                print(text.replace(BOL.INFORMATION,'[INFO]'))
                continue

            # DATA LINE
            # insert date and time
            text = '{:%Y-%m-%d,%H:%M:%S},'.format(datetime.datetime.now()) + text

            # print it
            print(text.replace(',','\t'))

            # write it
            datafile.write('{}\n'.format(text))
            datafile.flush()

            # count it
            counter += 1

    except KeyboardInterrupt:
        print("\nProgram was interrupted by user...")

    # close I/O
    ser.close()
    datafile.close()
    print("Streams have been closed.")

