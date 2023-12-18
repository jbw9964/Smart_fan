
import serial, pickle, time
import threading
from numpy import random

# define SERIAL_SPEED   115200
# define KEY_MSG_1      "Angle_left"                    // integer between [0 ~ 180]
# define KEY_MSG_2      "Angle_right"                   // integer between [0 ~ 180]
# define KEY_MSG_3      "Is_detected"                   // boolean
# define KEY_MSG_4      "QUIT"                          // boolean

MSG_dict = {
    "Angle_left"    :   0,
    "Angle_right"   :   180,
    "Is_detected"   :   False,
    "QUIT"          :   False
}

SERIAL_port = "COM3"
SERIAL_speed = 115200
SERIAL_class = None

TIME_delay = 10         # SEC
THREAD_flag = True

def randomize_data() : 
    global MSG_dict, TIME_delay, THREAD_flag

    print("Randomizing data...\n")

    try : 
        while THREAD_flag : 
            random_angle = random.randint(0, 180, size=2)
            MSG_dict["Angle_left"] = random_angle.min()
            MSG_dict["Angle_right"] = random_angle.max()
            MSG_dict["Is_detected"] = bool(random.randint(2, size=1)[0])
            
            print("[Angle_left] \t: {:3}".format(MSG_dict["Angle_left"]))
            print("[Angle_right] \t: {:3}".format(MSG_dict["Angle_right"]))
            print("[Is_detected] \t: {}\n".format(MSG_dict["Is_detected"]))

            time.sleep(TIME_delay)
    
    finally : 
        print("Shutting down sub thread...")
    
    print("Sub thread ended with no error\n")


if __name__ == "__main__" : 
    print("Testing Serial communication...\n")

    try : 
        SERIAL_class = serial.Serial(SERIAL_port, SERIAL_speed)
        
        thread = threading.Thread(target=randomize_data)
        thread.setDaemon(True)

        try : 
            thread.start()
            while True : 
                serialized_data = pickle.dumps(MSG_dict)
                SERIAL_class.write(serialized_data)
                time.sleep(TIME_delay)
        
        except KeyboardInterrupt : 
            print("\nShutting down program...\n")
            THREAD_flag = False

        thread.join()
    
    except : 
        print("Failed to connect serial with port : [ {} ]\n".format(SERIAL_port))
        exit(1)

    print("Main thread ended with no error\n")
    exit(0)
