
# ifndef __Bundle_sensor_led__
# define __Bundle_sensor_led__

# include <Arduino.h>
# include <base/base.h>

class LED_Bulk
{
    private:
        LED *LED_arr;
        int Cap;
        int Head;
        int index;

        bool is_full();
        void extend();

    public:
        LED_Bulk();
        LED_Bulk(int);
        ~LED_Bulk();

        void assign_port(int);
        int switch_level();
        void print_port();
};

class sensor_LED_bundle
{
    private:
        enum output_mode
        {
            digital = true,
            analog = false,
        };

        LED_Bulk *LED_bulk;
        Sensor *Input_sensor;

        int Current_level;
        int Max_level;

        int Port_output;
        bool output_flag;

        bool is_led_good;
        bool is_sensor_good;

        bool is_good();

    public:
        sensor_LED_bundle();
        sensor_LED_bundle(int, bool);
        ~sensor_LED_bundle();
        
        int level;
        
        void assign_led(int);
        void assign_input_sensor(int);
        void setup_output(int, bool);

        int process_signal();
        int process_signal(int);

        void print_port();
};

# ifndef __Bundle_sensor_led_func__
    # include <sensor_LED_bundle/sensor_LED_bundle.cpp>
# endif

# endif