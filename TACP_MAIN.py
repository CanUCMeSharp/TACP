import json
import serial
import time

def checksum(data):
    s = 0
    MOD_Checksum = 2 << 32
    for i in data:
        s = (s + i) % MOD_Checksum
    return s
#j = "{\"temp\":307,\"light\":249,\"loudness\":73,\"wind\":158,\"wDirection\":430,\"uv\":272,\"o2\":44,\"humidity\":378,\"pressure\":423,\"co2\":209,\"co\":440,\"no2\":165,\"smog\":492,\"currentTime\":85347}"
class Payload(object):
        def __init__(self, j):
            self.__dict__ = json.loads(j)


while True:
    try:
        ser = serial.Serial()
        ser.baudrate = 57600
        ser.port =   input("Serial port: ")
        ser.open()
        break
    except:
        print("No connection!")
while True:
    try:
        j = ser.readline()
        print(j)
        data = Payload(j)

        dataValues = [ int(data.temp), int(data.light), int(data.loudness), int(data.wind), int(data.wDirection), int(data.uv), int(data.o2), int(data.humidity), int(data.pressure), int(data.co2), int(data.co), int(data.no2), int(data.smog), int(data.ph1), int(data.ph2), int(data.ph3)]
        #print(int(data.checksum))
        #print(checksum(dataValues))
        #print()
        if int(data.checksum) != checksum(dataValues):
            ser.write(str(0x15).encode() + 0x0A.to_bytes(1, 'little'))
            #print("here")
            continue
        current_time = int(time.time())
        delta = current_time - int(data.time)
        if delta >= 2 or -delta >= 2:
            ser.write(str(0x07).encode() + 0x0A.to_bytes(1, 'little'))
            ser.write((str(current_time)).encode() + 0x0A.to_bytes(1, 'little'))
        #   print("here")
        else:
            ser.write(str(0x06).encode() + 0x0A.to_bytes(1, 'little'))
        #print(data.time)
        #print(data.ph1)

        #Do DB stuff
    except:
        try:
            ser.write(str(0x15).encode() + 0x0A.to_bytes(1, 'little'))
        except:
            try:
                ser.close()
                ser.open()
            except:
                print("Error in connection to Arduino")
                time.sleep(0.5)
