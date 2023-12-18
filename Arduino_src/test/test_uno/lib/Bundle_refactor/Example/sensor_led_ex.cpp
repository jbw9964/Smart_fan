
# include <Arduino.h>
# include <bundle_refactor.h>

enum LED_PORTS
{
    led1 = 2,
    led2 = 3,
    led3 = 4,
};

# define PORT_INPUT_SENSOR  5
# define PORT_OUTPUT        8

sensor_LED_bundle sensor_led_bundle;


void setup()
{
    Serial.begin(115200);
    while (!Serial) {delay(100);}
    delay(100);
    
    sensor_led_bundle.assign_led(led1);
    sensor_led_bundle.assign_led(led2);
    sensor_led_bundle.assign_led(led3);
    sensor_led_bundle.assign_input_sensor(PORT_INPUT_SENSOR);

    sensor_led_bundle.setup_output(PORT_OUTPUT, false);

    Serial.println("Done setup\n");
    sensor_led_bundle.print_port();
    Serial.println("\n");
}

void loop()
{
    int var = sensor_led_bundle.process_signal();
    // Serial.println(var);
    delay(100);

    if (Serial.available() && Serial.read() == 'q')
    {
        Serial.println("Exiting program...\n");
        delay(100);
        exit(0);
    }
}