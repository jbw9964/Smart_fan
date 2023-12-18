
import cv2, threading

class Videocapture_threading : 
    def __init__(self, source=0) :
        self.__PORT = source
        
        try : 
            self.__caputre = cv2.VideoCapture(self.__PORT)
            self.__is_opened = self.__caputre.isOpened()
            self.__caputre.set(cv2.CAP_PROP_BUFFERSIZE, 1)

        except : 
            print("Failed to open camera with port : [ {} ]".format(self.__PORT))
            self.__caputre = None
            self.__is_opened = False
        
        if self.__is_opened : 
            self.__status = self.__caputre.read()[0]
            self.__thread = threading.Thread(target=self.__read)
            self.__thread.setDaemon(True)

        else : 
            self.__status = False
            self.__thread = None
        
        if self.__status : 
            self.__HEIGHT, self.__WIDTH = self.__caputre.read()[1].shape[:2]
        
        else : 
            self.__HEIGHT, self.__WIDTH = None, None
        
        self.__frame = None
        self.__stop_flag = False
    
    def start_thread(self) : 
        if self.__thread : 
            self.__thread.start()
        
        else : 
            print("No thread was established. - status : [ {} ]".format(self.status))

    def __read(self) : 
        if not (self.__is_opened and self.__status) : 
            raise SystemError("Failed to read from camera with port : [ {} ]".format(self.__PORT))

        else : 
            print("Start receiving image...\n")

            while not self.__stop_flag and self.__is_opened : 
                (self.__status, self.__frame) = self.__caputre.read()
            
            print("End receiving image...\n")
    
    def stop_thread(self) : 
        self.__is_opened, self.__status = False, False
        self.__stop_flag = True
        if self.__thread and self.__thread.is_alive() : 
            self.__thread.join()

    def release(self) : 
        self.stop_thread()
        self.__caputre.release()
    
    def load_image(self) : 
        return self.__frame

    @property
    def status(self) : 
        if self.__is_opened and self.__status :     return True
        else :                                      return False

    @property
    def image_status(self) : 
        if type(self.__frame) == type(None) :   return False
        else :                                  return True
    
    @property
    def camera_property(self) : 
        return (self.__WIDTH, self.__HEIGHT)

import time
from ultralytics import YOLO

CAM_class = None
CAM_PORT = 0
WIDTH, HEIGHT = None, None

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
        while True : 
            time_total_start = time.time()
            frame = CAM_class.load_image()
            
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
            
            if cv2.waitKey(5) == ord('q') : 
                break
        
    except KeyboardInterrupt : 
        print("\n\nKeyboardInterrupt detected")

    if SAVE_IMG : 
        cv2.imwrite(filename=SAVE_IMG_path, img=pred_image)
    
    print("\nShutting down program...\n")


if __name__ == "__main__" : 
    flag = True
    print("\nConnecting to cammera...\n")

    CAM_class = Videocapture_threading(CAM_PORT)

    if CAM_class.status : 
        WIDTH, HEIGHT = CAM_class.camera_property
    
    
    if CAM_class.status : 
        print("Loading pretrained model...\n")
        try : 
            Model = YOLO(MODEL_PATH)
        
        except : 
            print("Failed to load pretrained model\n")
            flag = False

    if CAM_class.status and flag : 
        print("Start prediction...\n")
        CAM_class.start_thread()
        while not CAM_class.image_status : 
            time.sleep(0.1)
        
        main()

    else : 
        print("Aborting execution...\n")
    
    CAM_class.release()
    cv2.destroyAllWindows()

    print("Program exited with no error.\n")
    exit(0)