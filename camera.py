import cv2
from PIL import Image
cam_port = 0
cam = cv2.VideoCapture(cam_port)

result, image = cam.read()

if result:
    cv2.imwrite("Test.png", image)
    image = Image.open("Test.png")
    rotatedImage = image.rotate(270)
    rotatedImage.show()
    rotatedImage.save("Test.png")
