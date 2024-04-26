import cv2
from PIL import Image
from serial import Serial
import pytesseract
import numpy as np
import time

#break the letters into chunks
def divide_chunks(l,n):
    for i in range(0,len(l), n):
        yield l[i:i+n]


com_port = "COM15" #Change this as fit
cam_port = 0
filename = "Test.png"
flag = False
wheelNum = 10 #Number of Braille Wheels


esp32 = Serial(port = com_port,baudrate = 115200) #Initializing Serial connection to ESP32
cam = cv2.VideoCapture(cam_port) #Taking the image
result, image = cam.read()
imageTest = Image.open(filename)
if result:
    cv2.imwrite(filename, image)
    image = Image.open(filename) 
    rotatedImage = image.rotate(90) #Rotating the image into the correct orientation
    #rotatedImage = cv2.cvtColor(rotatedImage, cv2.COLOR_BGR2GRAY) #Grayscaling the image so that the text recognition hopefully works better
    rotatedImage.save(filename) #Resaving the new rotated/grayscaled image
rotatedImage.show()
# Starting to analyse the image and do OCR on it to pull out strings of words
img1 = np.array(rotatedImage)
text = pytesseract.image_to_string(img1)

text = text.lower() #Making the text lowercase to allow for indexing
print(text)
text = list(text) #Converting this string into a list

outputArray = np.zeros(len(text)) #Initializing the output array to the proper size
inputArray = ['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','.',',','?',' ','1','2','3','4','5','6','7','8','9','0']
for i in range(len(text)):
    try:
        outputArray[i] = inputArray.index(text[i])
    except ValueError: #if any value in the output from the image doesn't have a corresponding output on the braille wheel, it is discarded
        continue
#print(len(outputArray))
for i in range(len(outputArray)-1):
    if outputArray[i] == 0 and outputArray[i+1] == 0:
        outputArrayClean = np.delete(outputArray, i+1)
        flag = True
if flag:
    outputArray = outputArrayClean

print(outputArray)
outputArray = -outputArray
x = list(divide_chunks(outputArray,wheelNum))
print(len(x))
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
    step = int(step)

    if (step == 1):
        print("in forward")
        index = index + 1
    elif (step == -1):
        index = index - 1
    else: 
        index = index
    print("index is:", index)