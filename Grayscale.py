from serial import Serial
import time

port_num = "COM8"
esp32 = Serial(port = port_num, baudrate = 115200)

a = "1234567890"

esp32.write(bytes(a, "utf-8"))
data = esp32.readline().decode("utf-8")
print(data)