
# include <Arduino.h>
# include "../src/bundle_refactor.h"

# define PORT_INPUT_SENSOR  5
# define PORT_SERVO         8
# define L_max_angle        0
# define R_max_angle        180
# define UNIT_STEP          5

sensor_servo_bundle sensor_servo;


void setup()
{
    Serial.begin(115200);
    while (!Serial) {delay(100);}
    delay(100);

    sensor_servo.assign_input_sensor(PORT_INPUT_SENSOR);
    sensor_servo.assign_servo(PORT_SERVO, L_max_angle, R_max_angle, UNIT_STEP);
    sensor_servo.assign_angle(L_max_angle, R_max_angle, true);

    Serial.println("Done setup\n");
    sensor_servo.print_port();
    Serial.println("\n");
}

void loop()
{
    int var = sensor_servo.process_signal(true);
    // Serial.println(var);
    delay(100);

    if (Serial.available() && Serial.read() == 'q')
    {
        Serial.println("Exiting program...\n");
        delay(100);
        exit(0);
    }
}