import cv2
import urllib.request
import numpy as np
import socket

def colourdirection_left():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((esp32cam_ip, return_port))
    try:
        dir = 'l'
        client_socket.sendall(dir.encode())
    except Exception as e:
        print("Error connecting to ESP32CAM:", e)

def colourdirection_right():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((esp32cam_ip, return_port))
    try:
        dir = 'r'
        client_socket.sendall(dir.encode())
    except Exception as e:
        print("Error connecting to ESP32CAM:", e)

def nothing(x):
    pass

esp32cam_ip = '192.168.30.187'
esp32cam_port = 80
return_port = 88
url = f'http://{esp32cam_ip}:{esp32cam_port}/cam-hi.jpg'
cv2.namedWindow("live transmission", cv2.WINDOW_AUTOSIZE)

l_h, l_s, l_v = 0, 43, 46
u_h, u_s, u_v = 10, 255, 255

while True:
    img_resp = urllib.request.urlopen(url)
    imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
    frame = cv2.imdecode(imgnp, -1)
    # _, frame = cap.read()

    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    l_b = np.array([l_h, l_s, l_v])
    u_b = np.array([u_h, u_s, u_v])

    mask = cv2.inRange(hsv, l_b, u_b)

    cnts, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    for c in cnts:
        area = cv2.contourArea(c)
        if area > 2000:
            cv2.drawContours(frame, [c], -1, (255, 0, 0), 3)
            M = cv2.moments(c)
            cx = int(M["m10"] / M["m00"])
            cy = int(M["m01"] / M["m00"])
            if cx <= 390:#偏左
                colourdirection_left()
            elif cx >= 410:#偏右
                colourdirection_right()

            cv2.circle(frame, (cx, cy), 7, (255, 255, 255), -1)
            cv2.putText(frame, "red", (cx - 20, cy - 20), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)

    res = cv2.bitwise_and(frame, frame, mask=mask)

    cv2.imshow("live transmission", frame)
    cv2.imshow("mask", mask)
    cv2.imshow("res", res)
    key = cv2.waitKey(5)
    if key == ord('q'):
        break

cv2.destroyAllWindows()