from serial import Serial
import time

def divide_chunks(l,n):
    for i in range(0,len(l), n):
        yield l[i:i+n]
wheelNum = 10

com_port = "COM3"
esp32 = Serial(port = com_port, baudrate = 115200)
value = [2, 10, 30, 32, 28, 23, 10, 6, 18, 19, 14, 13, 13, 24]

x = list(divide_chunks(value,wheelNum))
index = 0

while 0<= index < len(x):
    for i in range(0,len(x[index])):
        print(i)
        dataSend = str(x[index][i])
        dataSend = dataSend + '\n'
        time.sleep(.0111)
        esp32.write(bytes(dataSend,"utf-8"))
    step = esp32.readline().decode("utf-8")
    print(step)
    try:
        step = int(step)
    except:
    print(step)

    if (step == 1):
        print("in forward")
        index = index + 1
    elif (step == -1):
        index = index - 1
    else: 
        print("here")
        index = index
    print("index is:", index)
        
