from PIL import Image
import pytesseract
import numpy as np
from serial import Serial
import time

# port_num = "COM8"
# esp32 = Serial(port = port_num, baudrate = 115200)

filename = "Test.png"
flag = 0
# Starting to analyse the image and do OCR on it to pull out strings of words
img1 = np.array(Image.open(filename))
text = pytesseract.image_to_string(img1)

text = text.lower() #Making the text lowercase to allow for indexing
text = list(text) #Converting this string into a list
outputArray = np.zeros(len(text)) #Initializing the output array to the proper size
inputArray = ['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','.',',','?',' ','1','2','3','4','5','6','7','8','9','0']
for i in range(len(text)):
    try:
        outputArray[i] = inputArray.index(text[i])
    except ValueError: #if any value in the output from the image doesn't have a corresponding output on the braille wheel, it is discarded
        continue
print(outputArray)
for i in range(len(outputArray)-1):
    if outputArray[i] == 0 and outputArray[i+1] == 0:
        outputArrayClean = np.delete(outputArray, i+1)
        flag = True
if flag:
    outputArray = outputArrayClean
outputArray = np.append(len(outputArray),outputArray) #Adding the length of array to initialize array in arduino
outputArray = np.append(100, outputArray) #Adding a known start point for the array that can be referenced
outputArray = np.append(outputArray, 101) #Adding a known end point for the array that can be referenced in arduino

# for i in range(len(outputArray)):
#     esp32.write(bytes(str(outputArray[i]),"utf-8"))
#     time.sleep(0.01)

# dataReceived = esp32.readline().decode("utf-8")

# print(dataReceived)