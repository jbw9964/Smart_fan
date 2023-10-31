
# ifndef __Raspberry_pi_JSON__
# define __Raspberry_pi_JSON__

# include <Arduino.h>
# include <ArduinoJson.h>

# define JSON_CAPACITY  (5 * JSON_OBJECT_SIZE(4))
# define SERIAL_SPEED   115200
# define KEY_MSG_1      "Angle_left"
# define KEY_MSG_2      "Angle_right"
# define KEY_MSG_3      "Is_detected"
# define KEY_MSG_4      "QUIT"

class Raspberry_pi_Json
{
    private:
        int Angle_Left;
        int Angle_Right;
        bool Detection_flag;

        DynamicJsonDocument *Doc;

        bool QUIT;
    
    public:
        Raspberry_pi_Json();
        Raspberry_pi_Json(int);
        ~Raspberry_pi_Json();

        int angle_Left;
        int angle_Right;
        bool detection_Flag;
        bool Quit;

        void begin_serial();
        void begin_serial(unsigned int);

        int receive_msg();

        void print_info();
};


# ifndef __Raspberry_pi_JSON_func__
    # include "./Raspberry_pi_Json.cpp"
# endif

# endif