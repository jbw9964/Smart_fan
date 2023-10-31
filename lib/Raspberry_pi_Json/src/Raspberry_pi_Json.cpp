
# ifndef __Raspberry_pi_JSON_func__
# define __Raspberry_pi_JSON_func__

# include <Raspberry_pi_Json.h>

/*
Raspberry_pi_Json:
    private:
        int Angle_Left;
        int Angle_Right;

        DynamicJsonDocument *Doc;
    
    public:
        Raspberry_pi_Json();
        Raspberry_pi_Json(int);
        ~Raspberry_pi_Json();

        int angle_left;
        int angle_right;

        int receive_msg();

        void print_info();
*/
Raspberry_pi_Json::Raspberry_pi_Json()
{
    Angle_Left = -1;
    Angle_Right = -1;
    Doc = new DynamicJsonDocument(JSON_CAPACITY);
}
Raspberry_pi_Json::Raspberry_pi_Json(int capacity)
{
    Angle_Left = -1;
    Angle_Right = -1;
    Doc = new DynamicJsonDocument(capacity);
}
Raspberry_pi_Json::~Raspberry_pi_Json()
{
    delete Doc;
}
void Raspberry_pi_Json::begin_serial()
{
    Serial.begin(SERIAL_SPEED);
    while (!Serial)     {delay(100);}
    delay(100);
}
void Raspberry_pi_Json::begin_serial(unsigned int speed)
{
    Serial.begin(speed);
    while (!Serial)     {delay(100);}
    delay(100);
}
int Raspberry_pi_Json::receive_msg()
{
    if (Serial.available() > 0)
    {
        String json_msg = Serial.readStringUntil('\n');
        
        DeserializationError err_msg = deserializeJson(*Doc, json_msg);

        if (err_msg)
        {
            Serial.print("[Error] Failed to deserialize Json message - ");
            Serial.println(err_msg.f_str());
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
    if (Serial)
    {
        Serial.print("[Angle left] : ");
        Serial.println(Angle_Left);
        Serial.print("[Angle right] : ");
        Serial.println(Angle_Right);
    }
}

# endif