
import cv2, time, serial, json

from ultralytics import YOLO

# ---------------------------------------------------------------------------------------------------------------- #
# Global variables

CAM_PORT = 0
CAM_class = None

Image = None
HEIGHT, WIDTH = None, None

SHOW_IMG = True
fontFace = cv2.QT_FONT_NORMAL
fontScale = 0.8
SAVE_IMG = True
IMG_PATH = "./test.png"

Model = None
MODEL_PATH = "./best.pt"
PRED_CONFIDENCE = 0.5
PRED_IMG_RESIZE = (320, 256)
VERBOSE = False

PERSON_CLS_INDEX = None
SQUARE_BOX_THRESHOLD_RATIO = 0.10
ANGLE_L = 90 - 25
ANGLE_R = 90 + 25
ANGLE_BIAS = 0

SERIAL_class = None
SERIAL_port = "/dev/ttyUSB0"
SERIAL_speed = 9600

# define SERIAL_SPEED   9600
# define KEY_MSG_1      "Angle_left"                    // integer between [0 ~ 180]
# define KEY_MSG_2      "Angle_right"                   // integer between [0 ~ 180]
# define KEY_MSG_3      "Is_detected"                   // boolean
# define KEY_MSG_4      "QUIT"                          // boolean

# ---------------------------------------------------------------------------------------------------------------- #
# Init functions

def load_model() : 
    global Model, MODEL_PATH

    try : 
        print("Loading pretrained model...\n")
        Model = YOLO(MODEL_PATH)                            # load model from path
        print("Successfully loaded pretrained model.\n")
    
    except :                # failed to load model
        print("Failed to load model at path : [ {} ]\n".format(MODEL_PATH))

def init_camera() : 
    global CAM_PORT, CAM_class
    global HEIGHT, WIDTH
    
    try :
        print("Connecting to cammera...\n")
        CAM_class = cv2.VideoCapture(CAM_PORT)          # connect camera
        CAM_class.set(cv2.CAP_PROP_BUFFERSIZE, 1)       # set minimum buffer size to load image
        print("Successfully connected to camera.\n")

    except :                    # failed to connect camera or something
        print("Failed to open camera with port : [ {} ]\n".format(CAM_PORT))

    if CAM_class and CAM_class.isOpened() :             # if camera were connected & is opened.
        assert CAM_class.get(cv2.CAP_PROP_BUFFERSIZE) in (1, 0), AssertionError("Buffer size is not 1.")        # check the buffersize
        
        ret, frame = CAM_class.read()           # load image
        
        if not ret :            # failed to load image
            print("Failed to read image from camera.\n")
        
        else :                  # set WIDTH & HEIGHT of camera
            HEIGHT, WIDTH = frame.shape[:2]
            print("Successfully read image from camera.\n")

def init_serial() : 
    global SERIAL_class, SERIAL_port, SERIAL_SPEED
    
    try :           # connect serial port
        SERIAL_class = serial.Serial(SERIAL_port, SERIAL_SPEED)
        print("Successfully loaded pretrained model.\n")
    
    except :        # failed to connect serial
        print("Failed to establish serial communication with port : [ {} ]\n".format(SERIAL_port))
        SERIAL_class = None

# ---------------------------------------------------------------------------------------------------------------- #
# Functions
    
def read_image() : 
    global CAM_class
    global Image

    Image = CAM_class.read()[1]         # store image
    Image = cv2.rotate(Image, cv2.ROTATE_90_COUNTERCLOCKWISE)   # rotate image & store

def preprocess_cls(ret=False) : 
    global Image, Model, VERBOSE, PERSON_CLS_INDEX

    if not Model : 
        print("Model were not loaded. Failed to preprocess classes.\n")
        return
    
    print("Preprocessing classes...\n")

    read_image()            # load image

    pred_result = Model.predict(source=Image, verbose=VERBOSE)[0]   # yolo prediction
    
    for index, cls_name in pred_result.names.items() : 
        if cls_name == "person" :                       # which cls index is human?
            PERSON_CLS_INDEX = index                    # store human cls index
            break

    print("Classes were preprocessed.\n")
    if ret : 
        return pred_result

def process_boxes(result_boxes) : 
    global WIDTH, HEIGHT
    global PERSON_CLS_INDEX, SQUARE_BOX_THRESHOLD_RATIO

    print("Processing result boxes...\n")

    pred_classes = result_boxes.cls.int().tolist()      # pull out predicted class
    pred_bboxes = result_boxes.xywh.int().tolist()      # pull out bbox coordinates

    valid_bboxes = []
    for cls, bbox in zip(pred_classes, pred_bboxes) : 
        if cls == PERSON_CLS_INDEX :                    # in pred result, if human exists,
            (central_x, central_y, w, h) = bbox         # pull out bbox coordinates of human
            x1 = int(central_x - w / 2)                 # calculate pt1
            y1 = int(central_y - h / 2)
            x2 = int(central_x + w / 2)                 # calculate pt2
            y2 = int(central_y + h / 2)

            if (w * h) / (WIDTH * HEIGHT) >= SQUARE_BOX_THRESHOLD_RATIO :       # human is close enough to camera
                valid_bboxes.append([x1, y1, x2, y2])

    print("Result boxes were filtered.\n")
    
    return valid_bboxes             # return result

def min_max_method(valid_bboxes) : 
    global HEIGHT
    global ANGLE_L, ANGLE_R, ANGLE_BIAS
    
    min_val = HEIGHT
    max_val = 0
    for bbox in valid_bboxes : 
        (x1, y1, x2, y2) = bbox     # pull out values
        left = x1
        right = x2

        if left < min_val :     min_val = left          # normalize left x coordiante
        if right > max_val :    max_val = right         # normalize right x coordiante

    min_val = int(min_val / HEIGHT * ANGLE_R + ANGLE_L - ANGLE_BIAS)        # calculate angle
    max_val = int(max_val / HEIGHT * ANGLE_R + ANGLE_L + ANGLE_BIAS)        # calcaulte angle

    if min_val < ANGLE_L :  min_val = ANGLE_L           # normalize left angle
    if max_val > ANGLE_R :  max_val = ANGLE_R           # normalize right angle

    return ((min_val + max_val) / 2 - 5, (min_val + max_val) / 2 + 5)       # return left & right angle (it's calibrated)

def send_json(angle_left, angle_right) : 
    global SERIAL_class, SERIAL_port

    print("Sending data via serial...\n")
    json_data = {                           # make data with JSON format
        "Angle_left"    :   angle_left,
        "Angle_right"   :   angle_right,
        "Is_detected"   :   True,
    }

    json_data = json.dumps(json_data) + "\n"
    serialized_data = json_data.encode("ascii")     # serialize data
    SERIAL_class.write(serialized_data)             # send data
    SERIAL_class.flush()

    print(json_data)                # show data
    print(serialized_data)

    print("Sucessfully sended data via serial [{}]".format(SERIAL_port))
    time.sleep(1)

def main() : 
    global CAM_class, Image, SHOW_IMG
    global Model, PRED_CONFIDENCE, PRED_IMG_RESIZE, VERBOSE
    global fontFace, fontScale
    

    try :                           # Loop while no interrution detected
        while True : 
            time_total_start = time.time()
            read_image()                        # load image from camera
            
            time_pred_start = time.time()
            Yolo_pred = Model.predict(          # predict Yolo with loaded image
                source=Image, conf=PRED_CONFIDENCE, imgsz=PRED_IMG_RESIZE, verbose=VERBOSE
            )
            time_pred_end = time.time()

            time_pred = time_pred_end - time_pred_start             # store execution time
            string_pred = "{:.3f} SEC, {:.3f} FPS".format(time_pred, 1 / time_pred)

            result_boxes = Yolo_pred[0].boxes                       # pull out prediction
            valid_bboxes = process_boxes(result_boxes)              # post-process the prediction

            if len(valid_bboxes) :                  # if human detected,
                print("Detected human\n")
                Angle_left, Angle_right = min_max_method(valid_bboxes)      # calcaulte left & right angle
                print("[ANGLE LEFT] \t: {}".format(Angle_left))
                print("[ANGLE RIGHT] \t: {}\n".format(Angle_right))
                send_json(Angle_left, Angle_right)                          # send data via serial communication
            
            else :                                  # no human detected
                print("No human were detected.\n")
            
            pred_image = Yolo_pred[0].plot()        # Yolo pred image
            cv2.putText(                            # plot execution time in pred image
                    pred_image, text=string_pred, org=(0, 20), 
                    fontFace=fontFace, fontScale=fontScale, color=(255, 255, 255), thickness=2,
                )
            cv2.putText(                            # plot execution time in pred image
                    pred_image, text="Total {:.3f} SEC, {:.3f} Execution per SEC".format(
                        time.time() - time_total_start, 1 / (time.time() - time_total_start)
                    ), 
                    org=(0, 50), 
                    fontFace=fontFace, fontScale=fontScale, color=(255, 255, 255), thickness=2,
                )
            
            if SHOW_IMG :                   # show image
                cv2.imshow(winname="Prediction", mat=pred_image)
                print(string_pred)
                print("Total {:.3f} SEC, {:.3f} Execution per SEC\n".format(
                    time.time() - time_total_start, 1 / (time.time() - time_total_start)
                ))
            
            if cv2.waitKey(25) == ord('q') :        # interrution detected
                print("Breaking loop...\n")
                break

    except KeyboardInterrupt :                      # interrution detected
        print("\n\n---> KeyboardInterrupt detected <---\n")
    
    except :                                        # error
        print("Unexpected error occured.\n")
    
    finally :                                       # dispose program
        print("\nShutting down program...\n")

        if SAVE_IMG :               # save latest frame
            print("Saving image to [ {} ]...\n".format(IMG_PATH))
            if cv2.imwrite(filename=IMG_PATH, img=pred_image) : 
                print("Image was saved at [ {} ].\n".format(IMG_PATH))
            
            else :                  # Failed to save frame
                print("Failed to save image to [ {} ]".format(IMG_PATH))

# ---------------------------------------------------------------------------------------------------------------- #
# Execution

if __name__ == "__main__" : 
    print("Start initializing...\n")

    load_model()
    init_camera()
    init_serial()
    preprocess_cls()

    if Model and CAM_class and SERIAL_class :           # Sucessfully initialized
        while Image is None : 
            time.sleep(0.1)
            read_image()
        
        print("[CAM WIDTH] \t: {}".format(WIDTH))
        print("[CAM HEIGHT] \t: {}\n".format(HEIGHT))
        
        print("\n---> Sucessfully initialized <---\n")
        print("\rStarting in 3...", end="")
        time.sleep(1)
        print("\rStarting in 2...", end="")
        time.sleep(1)
        print("\rStarting in 1...", end="")
        time.sleep(1)

        print("\n\n---> Start <---\n")
        main()                              # execute
        
        CAM_class.release()                 # disconnect camera
    
    else :                                              # Failed to initialize
        print(
            "Failed to start prediction : [Model : {}\tCAM_class : {}\tSERIAL_class : {}]\n".format(
                Model, CAM_class, SERIAL_class
            )
        )
    
    cv2.destroyAllWindows()

    print("Program ended with no error.\n")
    exit(0)
