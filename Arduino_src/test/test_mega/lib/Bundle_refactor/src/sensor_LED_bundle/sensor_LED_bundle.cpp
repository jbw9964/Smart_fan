
# ifndef __Bundle_sensor_led_func__
# define __Bundle_sensor_led_func__

# include <sensor_LED_bundle/sensor_LED_bundle.h>

/*
LED_bulk:
    private:
        LED *LED_arr;
        int Cap;
        int Head;
        int index;

        bool is_full();
        void extend();

    public:
        LED_Bulk();
        LED_Bulk(int capacity);
        ~LED_Bulk();

        void assign_port(int led_port);
        void switch_led();
        void print_port();
*/
LED_Bulk::LED_Bulk()                        // initialize LED_arr as Cap = 3
{
    Cap = 3;
    Head = 0;
    index = 0;

    if (LED_arr == nullptr)     {LED_arr = new LED[Cap];}
}
LED_Bulk::LED_Bulk(int capacity)            // initialize LED_arr with capacity;
{
    Cap = capacity;
    Head = 0;
    index = 0;

    if (LED_arr == nullptr)     {LED_arr = new LED[Cap];}
}
LED_Bulk::~LED_Bulk()                       // write LOW to LED & turn off
{
    // for (int i = 0; i < Head; i++)
    // {
    //     LED_arr[i].write_low();
    // }
    
    delete[] LED_arr;
    LED_arr = nullptr;
}
bool LED_Bulk::is_full()                    {return Cap <= Head ? true : false;}
void LED_Bulk::extend()                     // extend LED_arr
{
    LED *new_arr = new LED[Cap * 2];
    for (int i = 0; i < Head; i++)
    {
        new_arr[i] = LED_arr[i];
    }

    delete LED_arr;
    LED_arr = new_arr;
    Cap *= 2;
}
void LED_Bulk::assign_port(int led_port)    // assign LED's Port & setup
{
    if (is_full())          {extend();}
    LED_arr[Head++].assign_port(led_port);
}
int LED_Bulk::switch_level()                // cycle LED's
{
    if (index < Head)
    {
        LED_arr[index++].write_high();
    }
    else
    {
        index = 0;
        for (int i = 0; i < Head; i++)
        {
            LED_arr[i].write_low();
        }
    }

    return index;
}
void LED_Bulk::print_port()
{
    if (!Serial)    {return;}

    for (int i = 0; i < Head; i++)
    {
        Serial.print("[LED ");
        Serial.print(i + 1);
        Serial.print("] : ");
        LED_arr[i].print_port();
        delay(100);
    }
}


/*
sensor_LED_bundle:
    private:
        LED_Bulk LED_bulk;
        Sensor Input_sensor;
        int Current_level;
        int Max_level;

        enum output_mode
        {
            digital = true,
            analog = false,
        };

        bool output_flag;
        int Port_output;
        void setup_output();

        bool is_led_good;
        bool is_sensor_good;

        bool is_good();

    public:
        sensor_LED_bundle();
        sensor_LED_bundle(int, bool);
        ~sensor_LED_bundle();

        void assign_led(int);
        void assign_sensor();

        int process_signal();
        int process_signal(int);


        void print_port();
*/
sensor_LED_bundle::sensor_LED_bundle()
{
    Current_level = 0;
    Max_level = 0;
    output_flag = digital;
    Port_output = -1;

    LED_bulk = new LED_Bulk;
    Input_sensor = new Sensor;

    is_led_good = false;
    is_sensor_good = false;

    level = Current_level;
}
sensor_LED_bundle::sensor_LED_bundle(int port_output, bool is_digital)
{
    Current_level = 0;
    Max_level = 0;
    output_flag = is_digital;
    Port_output = port_output;

    LED_bulk = new LED_Bulk;
    Input_sensor = new Sensor;

    is_led_good = false;
    is_sensor_good = false;

    level = Current_level;
}
sensor_LED_bundle::~sensor_LED_bundle()
{
    delete LED_bulk;
    delete Input_sensor;
    if (output_flag == digital)     {digitalWrite(Port_output, LOW);}
    else                            {analogWrite(Port_output, 0);}
}
bool sensor_LED_bundle::is_good()               {return is_led_good && is_sensor_good ? true : false;}

void sensor_LED_bundle::assign_led(int led_port)
{
    is_led_good = true;
    LED_bulk->assign_port(led_port);
    Max_level++;
}
void sensor_LED_bundle::assign_input_sensor(int sensor_port)
{
    is_sensor_good = true;
    Input_sensor->assign_port(sensor_port);
}
void sensor_LED_bundle::setup_output(int port_output, bool is_digital)
{
    Port_output = port_output;
    output_flag = is_digital;
    pinMode(Port_output, OUTPUT);
}

int sensor_LED_bundle::process_signal(int prev_process)
{
    if (prev_process)
    {
        if (Serial)
        {
            Serial.println("[sensor_LED_bundle::process_signal] has been aborted, since previous process was corrupted.");
        }

        return 1;
    }

    return process_signal();
}
int sensor_LED_bundle::process_signal()
{
    if (!is_good())     {return 1;}

    if (Input_sensor->is_received())
    {
        Current_level = LED_bulk->switch_level();

        if (output_flag == analog)
        {
            int val = map(Current_level, 0, Max_level, 0, 250);
            analogWrite(Port_output, val);
        }
        else if (Current_level)
        {
            digitalWrite(Port_output, HIGH);
        }
        else
        {
            digitalWrite(Port_output, LOW);
        }

        level = Current_level;
    }

    return 0;
}

void sensor_LED_bundle::print_port()
{
    LED_bulk->print_port();
    
    if (Serial)
    {
        Serial.print("[Input sensor] : ");
        Input_sensor->print_port();
    }
}

# endif