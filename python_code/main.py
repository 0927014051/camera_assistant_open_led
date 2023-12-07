import cv2
import argparse
from ultralytics import YOLO
import supervision as sv
import serial


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="YOLOv8 live")
    parser.add_argument(
        "--webcam-resolution",
        default=[1200,720],
        nargs=2,
        type=int
    )
    args = parser.parse_args()
    return args
def main():
    ser = serial.Serial("COM3", 9600)

    args = parse_arguments()
    frame_width, frame_height = args.webcam_resolution

    cap = cv2.VideoCapture(0)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, frame_width)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, frame_height)

    model = YOLO("yolov8n.pt")

    box_annotator = sv.BoxAnnotator(
        thickness=2,
        text_thickness=2,
        text_scale=1
    )
    tmp = "person"

    while True:
        check = 'f'
        ret, frame = cap.read()

        results = model(frame, show = True, conf = 0.4)[0]

        # detections = sv.Detections.from_ultralytics(results)
        names = model.names

        for r in results:
            for c in r.boxes.cls:
                # print(names[int(c)])
                if (names[int(c)] == tmp):
                    check = 't'
        print(check)
        ser.write(check.encode())
        if (cv2.waitKey(30) == 27):
            break
if __name__ == '__main__':
    main()
