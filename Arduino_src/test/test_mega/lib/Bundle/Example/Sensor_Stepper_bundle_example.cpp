
# include "../src/Bundle.h"

Sensor_Stepper_bundle temp;

int total_step = 200;
int pin1 = 2;
int pin2 = 3;
int pin3 = 4;
int pin4 = 5;

int sensor_port = 6;

int current_angle = 0;
int r_max = 60;
int l_max = -60;
unsigned int speed = 5;

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    delay(100);

    temp.assign_motor(total_step, pin1, pin2, pin3, pin4);
    temp.assign_sensor(sensor_port);
    temp.assign_angle(current_angle, r_max, l_max);
    temp.set_speed(speed);

    Serial.println("done setup");
}

void loop()
{
    int var = temp.process_signal();
    Serial.println(var);
    delay(100);
}