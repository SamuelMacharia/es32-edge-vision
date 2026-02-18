import cv2
from ultralytics import YOLO
import imutils
from imutils import face_utils

print(cv2.__version__)


url = "http://192.168.100.91/stream"


model = YOLO("ai/models/yolov8n-face-lindevs.pt")
model.export(format="onnx")

capture = cv2.VideoCapture(url)

while True:
    ret, frame = capture.read()
    if not ret:
        print("failed to retrieve capture")
        break

    results = model(frame, imgsz=192) #tune imgsz depending on the fps speed, try 256, 320 or 640

    for i in results:
        boxes = i.boxes.xyxy

        for box in boxes:
            x1, y1, x2, y2 = map(int, box)
            roi = frame[y1:y2, x1:x2]

            cv2.rectangle(frame, (x1,y1), (x2,y2), (0, 255, 0),2)

            #send roi to face recognizer
            roi= cv2.resize(roi, (160, 160))
    cv2.imshow("HTTP LIVE Stream", frame)

    if cv2.waitKey(1) == ord('q'):
        break

capture.release()
cv2.destroyAllWindows()