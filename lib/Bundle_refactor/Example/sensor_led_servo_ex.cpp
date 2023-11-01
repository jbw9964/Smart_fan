
# include <Arduino.h>
# include "../src/bundle_refactor.h"

enum LED_PORTS
{
    led1 = 2,
    led2 = 3,
    led3 = 4,
};

# define PORT_INPUT_SENSOR_1    5
# define PORT_MOTOR             10

# define PORT_INPUT_SENSOR_2    6
# define PORT_SERVO             8
# define L_max_angle            0
# define R_max_angle            180
# define UNIT_STEP              5

sensor_LED_bundle sensor_led_bundle;
sensor_servo_bundle sensor_servo;


void setup()
{
    Serial.begin(115200);
    while (!Serial) {delay(100);}
    delay(100);
    
    sensor_led_bundle.assign_led(led1);
    sensor_led_bundle.assign_led(led2);
    sensor_led_bundle.assign_led(led3);
    sensor_led_bundle.assign_input_sensor(PORT_INPUT_SENSOR_1);

    sensor_led_bundle.setup_output(PORT_MOTOR, false);

    sensor_servo.assign_input_sensor(PORT_INPUT_SENSOR_2);
    sensor_servo.assign_servo(PORT_SERVO, L_max_angle, R_max_angle, UNIT_STEP);
    sensor_servo.assign_angle(L_max_angle, R_max_angle, true);

    Serial.println("Done setup\n");
    sensor_led_bundle.print_port();
    Serial.println("\n");
    sensor_servo.print_port();
    Serial.println("\n");
    delay(2000);
}

void loop()
{
    int var1 = sensor_led_bundle.process_signal();

    int var2;
    if (sensor_led_bundle.level)
    {
        var2 = sensor_servo.process_signal();
    }
    
    // Serial.println(var1);
    // Serial.println(var2);
    delay(100);

    if (Serial.available() && Serial.read() == 'q')
    {
        Serial.println("Exiting program...\n");
        delay(100);
        exit(0);
    }
}