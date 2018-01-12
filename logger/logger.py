##
# 
# by Louw Pretorius
##

import serial
import time
import datetime

import log_settings as settings


class EOL:
    DATA = '$'
    DEVICE = '&'


def makeFile():
    filename = settings.OUT_FILENAME_FORMAT.format(datetime.datetime.now())
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
            
            # Device reading
            if text[0] == EOL.DEVICE:
                # further cleaning
                text = text.replace(EOL.DEVICE, '')

                # print it
                print('DEVICE {} at {}'.format(deviceIndex, text))

                # write it
                datafile.write('DEVICE {} at {}\n'.format(deviceIndex, text))
                datafile.flush()

                # count it
                deviceIndex += 1

                # next it
                continue

            # data reading
            if text[0] == EOL.DATA:
                # further cleaning
                text = text.replace(EOL.DATA, '')

                # insert date and time
                text = text.replace('@', '{:%Y-%m-%d,%H:%M:%S}'.format(datetime.datetime.now()))

                # header it
                if counter == 0:
                    print('\nID\tDATE\t\tTIME\t\tREADINGS')

                # print it
                print(text.replace(',','\t'))

                # write it
                datafile.write('{}\n'.format(text))
                datafile.flush()

                # count it
                counter += 1

                # next it
                continue

    except KeyboardInterrupt:
        print("\nProgram was interrupted by user...")

    # close I/O
    ser.close()
    datafile.close()
    print("Streams have been closed.")

