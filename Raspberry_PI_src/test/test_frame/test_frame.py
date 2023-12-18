
import cv2, time
from ultralytics import YOLO

CAM_class = None
CAM_PORT = 0
HEIGHT, WIDTH = None, None

SHOW_IMG = True
fontFace = cv2.QT_FONT_NORMAL
fontScale = 0.8

Model = None
MODEL_PATH = "../yolov8n.pt"
VERBOSE = False

SAVE_IMG_path = "./test_image.jpg"
SAVE_IMG = True


def main() : 
    global CAM_class, SHOW_IMG, fontFace, fontScale, Model

    try : 
        while CAM_class.isOpened() : 
            time_total_start = time.time()
            ret, frame = CAM_class.read()

            if not ret : 
                print("Failed to read image from camera.\n")
                break
            
            time_pred_start = time.time()
            Yolo_pred_box = Model.predict(frame, verbose=VERBOSE)
            time_pred_end = time.time()

            time_pred = time_pred_end - time_pred_start
            string_pred = "{:.3f} SEC, {:.3f} FPS".format(time_pred, 1 / time_pred)
            
            pred_image = Yolo_pred_box[0].plot()
            cv2.putText(
                    pred_image, text=string_pred, org=(0, 20), 
                    fontFace=fontFace, fontScale=fontScale, color=(255, 255, 255), thickness=2,
                )
            cv2.putText(
                    pred_image, text="Total {:.3f} SEC, {:.3f} Execution per SEC".format(
                        time.time() - time_total_start, 1 / (time.time() - time_total_start)
                    ), 
                    org=(0, 50), 
                    fontFace=fontFace, fontScale=fontScale, color=(255, 255, 255), thickness=2,
                )
            
            if SHOW_IMG : 
                cv2.imshow(winname="Prediction", mat=pred_image)
                print(string_pred)
                print("Total {:.3f} SEC, {:.3f} Execution per SEC\n".format(
                    time.time() - time_total_start, 1 / (time.time() - time_total_start)
                ))
            
            if cv2.waitKey(25) == ord('q') : 
                break
        
    except KeyboardInterrupt : 
        pass

    if SAVE_IMG : 
        cv2.imwrite(filename=SAVE_IMG_path, img=pred_image)
    
    print("\n\nShutting down program...\n")


if __name__ == "__main__" : 
    flag = True
    print("\nConnecting to cammera...\n")

    CAM_class = cv2.VideoCapture(CAM_PORT)
    CAM_class.set(cv2.CAP_PROP_BUFFERSIZE, 1)

    if not CAM_class.isOpened() : 
        print("Failed to open camera with port : [ {} ]\n".format(CAM_PORT))
        flag = False
    
    if not CAM_class.read()[0] : 
        print("Failed to read image from camera.\n")
        flag = False
    
    else : 
        HEIGHT, WIDTH = CAM_class.read()[1].shape[:2]
    
    if flag : 
        print("Loading pretrained model...\n")
        try : 
            Model = YOLO(MODEL_PATH)
        
        except : 
            print("Failed to load pretrained model\n")
            flag = False

    if flag : 
        print("Start prediction...\n")
        main()

    else : 
        print("Aborting execution...\n")
    
    CAM_class.release()
    cv2.destroyAllWindows()

    print("Program exited with no error.\n")
    exit(0)