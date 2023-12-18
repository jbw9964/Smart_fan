
# ifndef __Raspberry_pi_JSON_func__
# define __Raspberry_pi_JSON_func__

# include <Raspberry_pi_Json.h>

/*
Raspberry_pi_Json:
    private:
        int Angle_Left;
        int Angle_Right;
        bool Detection_flag;

        DynamicJsonDocument *Doc;
        HardwareSerial *Input_serial;

        bool QUIT;
    
    public:
        Raspberry_pi_Json();
        Raspberry_pi_Json(HardwareSerial);
        Raspberry_pi_Json(int);
        Raspberry_pi_Json(HardwareSerial, int);
        ~Raspberry_pi_Json();

        int angle_Left;
        int angle_Right;
        bool detection_Flag;
        bool Quit;

        void begin_serial();
        void begin_serial(unsigned int);
        void begin_serial(HardwareSerial, unsigned int);

        int receive_msg();

        void print_info();
*/
Raspberry_pi_Json::Raspberry_pi_Json()
{
    Angle_Left = -1;
    Angle_Right = -1;
    Doc = new DynamicJsonDocument(JSON_CAPACITY);
    
    Input_serial = &Serial;
    // Output_serial = &Serial1;
    Output_serial = &Serial;
}
Raspberry_pi_Json::Raspberry_pi_Json(HardwareSerial *input_serial, HardwareSerial *output_serial)
{
    Angle_Left = -1;
    Angle_Right = -1;
    Doc = new DynamicJsonDocument(JSON_CAPACITY);
    
    Input_serial = input_serial;
    Output_serial = output_serial;
}
Raspberry_pi_Json::Raspberry_pi_Json(unsigned int capacity)
{
    Angle_Left = -1;
    Angle_Right = -1;
    Doc = new DynamicJsonDocument(capacity);

    Input_serial = &Serial;
    // Output_serial = &Serial1;
    Output_serial = &Serial;
}
Raspberry_pi_Json::Raspberry_pi_Json(HardwareSerial *input_serial, HardwareSerial *output_serial, unsigned int capacity)
{
    Angle_Left = -1;
    Angle_Right = -1;
    Doc = new DynamicJsonDocument(capacity);

    Input_serial = input_serial;
    Output_serial = output_serial;
}

Raspberry_pi_Json::~Raspberry_pi_Json()
{
    delete Doc;
    Input_serial = nullptr;
    Output_serial = nullptr;
}
void Raspberry_pi_Json::begin_serial()
{
    Input_serial->begin(SERIAL_SPEED);
    while (!Input_serial)     {delay(100);}
    delay(100);

    Output_serial->begin(SERIAL_SPEED);
    while (!Output_serial)     {delay(100);}
    delay(100);
}
void Raspberry_pi_Json::begin_serial(unsigned int speed)
{
    Input_serial->begin(speed);
    while (!Input_serial)     {delay(100);}
    delay(100);

    Output_serial->begin(speed);
    while (!Output_serial)     {delay(100);}
    delay(100);
}
int Raspberry_pi_Json::receive_msg()
{
    if (Input_serial->available() > 0)
    {
        String json_msg = Input_serial->readStringUntil('\n');
        Serial.println(json_msg);
        
        DeserializationError err_msg = deserializeJson(*Doc, json_msg);

        if (err_msg)
        {
            Output_serial->print("[Error] Failed to deserialize Json message - ");
            Output_serial->println(err_msg.f_str());
        }
        else
        {
            Angle_Left = (*Doc)[KEY_MSG_1];
            Angle_Right = (*Doc)[KEY_MSG_2];
            Detection_flag = (*Doc)[KEY_MSG_3];
            QUIT = (*Doc)[KEY_MSG_4];

            angle_Left = Angle_Left;
            angle_Right = Angle_Right;
            detection_Flag = Detection_flag;
            Quit = QUIT;
        }

        return 1;
    }

    return 0;
}
void Raspberry_pi_Json::print_info()
{
    if (Output_serial)
    {
        Output_serial->print("[Angle left] : ");
        Output_serial->println(Angle_Left);
        Output_serial->print("[Angle right] : ");
        Output_serial->println(Angle_Right);
    }
}

# endif