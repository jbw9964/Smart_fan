
# ifndef __Sensor_LED_bundle__
# define __Sensor_LED_bundle__ "IMPLEMENTED"

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

class Sensor_LED_bundle
{
    private :
        LED_Bulk LED_bulk;
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
        Sensor_LED_bundle();
        Sensor_LED_bundle(int port_output, bool digital, bool analog);
        ~Sensor_LED_bundle()
        {
            delete &LED_bulk;
            delete &Power_sensor;
        }

        void assign_led(int led_port);
        void assign_sensor(int sensor_port);

        int process_signal();

        void print_port();
};

# ifndef __Sensor_LED_bundle_func__
# include "Sensor_LED_bundle.cpp"
# endif

# endif