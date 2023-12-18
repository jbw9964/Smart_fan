
import cv2, time

if __name__ == "__main__" : 
    CAM_class = None
    CAM_port = 0
    CAM_class = cv2.VideoCapture(CAM_port)
    CAM_class.set(cv2.CAP_PROP_BUFFERSIZE, 1)

    if CAM_class.isOpened() : 
        
        while CAM_class.isOpened() : 
            ret, frame = CAM_class.read()

            if not ret : 
                print("Failed to load image from camera.")
                break
            
            cv2.imshow(winname="window", mat=frame)

            if cv2.waitKey(25) == ord('q') : 
                print("Shutting down window...")
                break

    else : 
        print("Failed to open camera with port [ {} ]".format(CAM_port))

    CAM_class.release()

    print("Program exited with no error.\n")