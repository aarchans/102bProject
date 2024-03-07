import cv2
from PIL import Image
from serial import Serial
import pytesseract
import numpy as np

# com_port = "COM8"
cam_port = 0
filename = "Test.png"
flag = False
# Taking the image
# esp32 = Serial(port = com_port,baudrate = 115200)
cam = cv2.VideoCapture(cam_port)
result, image = cam.read()

if result:
    cv2.imwrite(filename, image)
    image = Image.open(filename) 
    rotatedImage = image.rotate(270) #Rotating the image into the correct orientation
    #rotatedImage = cv2.cvtColor(rotatedImage, cv2.COLOR_BGR2GRAY) #Grayscaling the image so that the text recognition hopefully works better
    rotatedImage.save(filename) #Resaving the new rotated/grayscaled image

# Starting to analyse the image and do OCR on it to pull out strings of words
img1 = np.array(Image.open(filename))
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
outputArray = np.append(100, outputArray) #Adding a known start point for the array that can be referenced
outputArray = np.append(outputArray, [-1]) #Adding a known end point for the array that can be referenced in arduino
print(outputArray)
# for i in range(len(outputArray)):
#     esp32.write(bytes(str(outputArray[i]),"utf-8"))

# dataReceived = esp32.readline().decode("utf-8")

# print(dataReceived)

