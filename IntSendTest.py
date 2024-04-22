from serial import Serial
import time

wheelNum = 10

com_port = "COM9"
esp32 = Serial(port = com_port, baudrate = 115200)
value = [2, 10, 30, 32, 28, 23, 10, 6, 18, 19, 14, 13, 13, 24]

for i in range(len(value)):
    dataSend = str(value[i])
    dataSend = dataSend + '\n'
    time.sleep(.0111)
    esp32.write(bytes(dataSend,"utf-8"))

dataReceived = esp32.readline().decode("utf-8")
print(dataReceived)