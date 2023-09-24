
# ifndef __LED_Button_bundle__
# define __LED_Button_bundle__ "IMPLEMENTED"

# include <Arduino.h>
# include "./Base.h"

class LED_Bulk
{
    private : 
        LED *LED_arr;
        int Cap;
        int Head;
        int index;

        bool is_full()              {return Cap <= Head ? true : false;}
        void extend()
        {
            LED *new_arr = new LED[Cap * 2];
            for (int i = 0; i < Cap; i++)
            {
                new_arr[i] = LED_arr[i];
            }

            delete[] LED_arr;
            delete LED_arr;

            LED_arr = new_arr;
            Cap *= 2;
        }
    
    public : 
        LED_Bulk();
        LED_Bulk(int capacity);
        ~LED_Bulk();

        void assign_port(int led_port);
        void switch_led();
        void print_port();
};
LED_Bulk::LED_Bulk()
{
    delete LED_arr;
    Cap = 3;
    Head = 0;
    index = 0;

    LED_arr = new LED[Cap];
}
LED_Bulk::LED_Bulk(int capacity)
{
    delete LED_arr;
    Cap = capacity;
    Head = 0;
    index = 0;

    LED_arr = new LED[capacity];
}
LED_Bulk::~LED_Bulk()
{
    for (int i = 0; i < Cap; i++)
    {
        LED_arr[i].pub_low();
    }
    
    delete[] LED_arr;
    delete LED_arr;
    LED_arr = nullptr;
}
void LED_Bulk::assign_port(int led_port)
{
    if (is_full())      {extend();}

    LED_arr[Head++].assign_port(led_port);
}
void LED_Bulk::switch_led()
{
    if (index >= Head)
    {
        index = 0;
        for (int i = 0; i < Head; i++)
        {
            LED_arr[i].pub_low();
        }
    }
    else
    {
        LED_arr[index++].pub_high();
    }
}
void LED_Bulk::print_port()
{
    for (int i = 0; i < Head; i++)
    {
        Serial.print("LED ");
        delay(10);
        Serial.print(i + 1);
        delay(10);
        Serial.print(" : ");
        delay(10);
        LED_arr[i].print_port();
        delay(10);
    }
}

class LED_Button_bundle
{
    private :
        LED_Bulk LED_Bundle;
        Sensor Power_sensor;
        int Current_level;          // 0 <= Current_level <= Max_level : 
        int Max_level;              // Number of levels : Max_level + 1

        bool flag_digital;
        bool flag_analog;
        int Port_output;
        void setup()                {return pinMode(Port_output, OUTPUT);}

        bool led_flag;
        bool sensor_flag;

        bool is_good()              {return led_flag && sensor_flag ? true : false;}
    
    public :
        LED_Button_bundle();
        LED_Button_bundle(int port_output, bool digital, bool analog);
        ~LED_Button_bundle()
        {
            delete &LED_Bundle;
            delete &Power_sensor;
        }

        void assign_led(int led_port);
        void assign_sensor(int sensor_port);

        int process_signal();

        void print_port();
};
LED_Button_bundle::LED_Button_bundle()
{
    Current_level = 0;
    Max_level = 0;
    flag_digital = true;
    flag_analog = false;
    Port_output = -1;
}
LED_Button_bundle::LED_Button_bundle(int port_output, bool digital, bool analog)
{
    Current_level = 0;
    Max_level = 0;

    Port_output = port_output;
    setup();
    if (analog)
    {
        flag_digital = false;
        flag_analog = true;
    }
    else
    {
        flag_digital = true;
        flag_analog = false;
    }
}
void LED_Button_bundle::assign_led(int led_port)
{
    led_flag = true;
    LED_Bundle.assign_port(led_port);
    Max_level++;
}
void LED_Button_bundle::assign_sensor(int sensor_port)
{
    sensor_flag = true;
    Power_sensor.assign_port(sensor_port);
}
int LED_Button_bundle::process_signal()
{
    if (!is_good())     {return 1;}

    if (Power_sensor.is_received())
    {
        LED_Bundle.switch_led();
        Current_level = Current_level >= Max_level ? 0 : Current_level + 1;

        if (flag_analog)
        {
            analogWrite(Port_output, map(Current_level, 0, Max_level, 0, 255));
        }
        else if (Current_level == 0)
        {
            digitalWrite(Port_output, LOW);
        }
        else
        {
            digitalWrite(Port_output, HIGH);
        }
    }
    
    return 0;
}
void LED_Button_bundle::print_port()
{
    LED_Bundle.print_port();
    Serial.print("Sensor : ");
    delay(10);
    Power_sensor.print_port();
    delay(10);
}

# endif