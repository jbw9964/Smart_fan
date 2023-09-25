
# define PORT_SENSOR 2
# define PORT_MOT 3

# include "../Bundle/Sensor_Servo_bundle.h"

Sensor_Servo_bundle Servo_bundle;

int unit_step = 5;
int l_max = 0;
int r_max = 180;

void setup()
{
    Servo_bundle.assign(PORT_SENSOR, PORT_MOT, unit_step);
    Servo_bundle.assign_angle(l_max, r_max);
}

void loop()
{
    int var = Servo_bundle.process_signal();

    delay(100);
}

