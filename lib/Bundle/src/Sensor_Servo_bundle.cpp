
# ifndef __Sensor_Servo_bundle_func__
# define __Sensor_Servo_bundle_func__

# ifndef __Sensor_Servo_bundle__
# include "Sensor_Servo_bundle.h"
# endif

bool Sensor_Servo_bundle::is_good()
{
    return ((Current_angle != -1) && (Unit_step != -1)) ? true : false;
}
Sensor_Servo_bundle::Sensor_Servo_bundle()
{
    R_max_angle = 180;
    L_max_angle = 0;
    Current_angle = -1;
    Mode = Stop;
    direction = Right;
    Unit_step = -1;
}
Sensor_Servo_bundle::~Sensor_Servo_bundle()
{
    delete &Servo_motor;
    delete &Power_sensor;
}

void Sensor_Servo_bundle::assign(int sensor_port, int motor_port, int unit_step)
{
    assign_sensor(sensor_port);
    assign_motor(motor_port, unit_step);
}
void Sensor_Servo_bundle::assign_sensor(int sensor_port)
{
    return Power_sensor.assign_port(sensor_port);
}
int Sensor_Servo_bundle::assign_motor(int motor_port, int unit_step)
{
    if (!Servo_motor.attached())
    {
        Servo_motor.attach(motor_port);
        Current_angle = Servo_motor.read();
        Unit_step = unit_step;
        return 0;
    }
    return 1;
}
void Sensor_Servo_bundle::assign_angle(int l_max, int r_max)
{
    L_max_angle = abs(l_max);
    R_max_angle = abs(r_max);
    Current_angle = Servo_motor.read();
    Serial.println(Current_angle);
}
int Sensor_Servo_bundle::process_signal()
{
    if (!is_good())                     {return 1;}

    if (Power_sensor.is_received())     {Mode = !Mode;}

    if (Mode == Spin)
    {
        if (direction == Right)
        {
            Current_angle += Unit_step;
        }
        else
        {
            Current_angle -= Unit_step;
        }

        if (L_max_angle >= Current_angle)
        {
            direction = Right;
            Current_angle += Unit_step;
        }
        else if (R_max_angle <= Current_angle)
        {
            direction = Left;
            Current_angle -= Unit_step;
        }
        
        Servo_motor.write(Current_angle);
    }

    return 0;
}

# endif