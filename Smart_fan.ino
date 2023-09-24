
# define PORT_SENSOR 5
# define PORT_OUTPUT 8

# include "./Bundle/LED_Button_bundle.h"
# include "./Bundle/Rotate_button_bundle.h"

LED_Button_bundle temp(PORT_OUTPUT, true, false);

void setup()
{
	temp.assign_led(2);
    temp.assign_led(3);
    temp.assign_led(4);
    temp.assign_sensor(PORT_SENSOR);

    Serial.begin(9600);
    while (!Serial)

    Serial.println("done setup\n");
}

void loop()
{
    int var = temp.process_signal();
	// Serial.println(var);
    delay(100);
}