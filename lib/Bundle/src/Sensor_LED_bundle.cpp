
# ifndef __Sensor_LED_bundle_func__
# define __Sensor_LED_bundle_func__

# ifndef __Sensor_LED_bundle__
    # include "./Sensor_LED_bundle.h"
# endif

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

Sensor_LED_bundle::Sensor_LED_bundle()
{
    Current_level = 0;
    Max_level = 0;
    flag_digital = true;
    flag_analog = false;
    Port_output = -1;
}
Sensor_LED_bundle::Sensor_LED_bundle(int port_output, bool digital, bool analog)
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
void Sensor_LED_bundle::assign_led(int led_port)
{
    led_flag = true;
    LED_bulk.assign_port(led_port);
    Max_level++;
}
void Sensor_LED_bundle::assign_sensor(int sensor_port)
{
    sensor_flag = true;
    Power_sensor.assign_port(sensor_port);
}
int Sensor_LED_bundle::process_signal()
{
    if (!is_good())     {return 1;}

    if (Power_sensor.is_received())
    {
        LED_bulk.switch_led();
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
void Sensor_LED_bundle::print_port()
{
    LED_bulk.print_port();
    Serial.print("Sensor : ");
    delay(10);
    Power_sensor.print_port();
    delay(10);
}

# endif