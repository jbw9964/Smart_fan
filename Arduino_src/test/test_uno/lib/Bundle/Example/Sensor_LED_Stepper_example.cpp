
# include <Arduino.h>
# include "../src/Bundle.h"

enum PORT_LED
{
    led_1 = 2,
    led_2,
    led_3,
};
enum PORT_virtual_motor
{
    motor = 6,
};
enum PORT_button
{
    button_1 = 5,
    button_2 = 7,
};
enum PORT_stepper
{
    A_plus = 8,
    A_minus,
    B_plus,
    B_minus,
};

/*
    [LED_Button_bundle class instructions]

    Constructor
    --------------------------------------------------------------------------------------------------------
    LED_Button_bundle(int port_output, bool digital, bool analog)
    - determine spinning motor's port (`port_output`)
    - determine whether `port_output` as digital or analog

    Initialize
    --------------------------------------------------------------------------------------------------------
    After create instance, you need to...
    - 1. assign_led(int led_port)
    - 2. assign_sensor(int sensor_port)

    1. void assign_led(int led_port)
    - Assign `LED` instance with `led_port`. 
    - If you have 3 LEDs, you have to call this method 3 times.
    - Calling this method will set the number of levels of `port_output`.
    - For example, if you call method 3 times, it will automatically set 4 levels.
    - level 0 : Power off   /   level 1     /   level 2     /   level 3

    2. void assign_sensor(int sensor_port)
    - Assign `Sensor` instance with `sensor_port`.
    - The `Sensor` instance will recognize the input signal of `sensor_port`.
    - However, `Sensor` instance only alert when signal turns HIGH to LOW. (like push button)
    - So if your sensor has only On/Off switch, it will recognize when it's switch On to Off (On --> OFF)

    Receive and publish signals
    --------------------------------------------------------------------------------------------------------
    After initialize, you can use `process_signal` method to receive `sensor_port`s signal, and publish signal to `led_port`s.
    
    --> int process_signal() 
    - Check whether sinal has been received.
    - If it received signal, switch internal power levels, and shows via LEDs
    - By "pushing" `Sensor`, the power levels and number of LEDs will circulate.
    - For example, if you have assigned 3 LED, the power levels and number of LEDs will be like down below.

    Initial state       /   Pushed once /   Pushed twice    /   Pushed third    /   Pushed fourth   /   ...Circulate...
    Power level :   0           1               2                   3                   0
    LED number  :   0           1               2                   3                   0

    - NOTICE
    If either `Sensor` OR LED were NOT initialized, `process_signal` method WILL BE ABORTED.
    If `Sensor` OR LED were not initialized, `process_signal` will only returns 1 and not switchs power or LED.
    If not, returns 0 and switch power and LEDs.
    --------------------------------------------------------------------------------------------------------

    Dependency  :   None
*/

/*
    [Rotate_button_bundle class instructions]

    Constructor
    --------------------------------------------------------------------------------------------------------
    Rotate_button_bundle::Rotate_button_bundle()
    - create invalid instance first

    Initialize
    --------------------------------------------------------------------------------------------------------
    After create instance, you need to...
    - 1. assign_motor(int total_steps, int A_plus, int A_minus, int B_plus, int B_minus)
    - 2. assign_sensor(int sensor_port)
    - 3. assign_angle(int current_angle, int r_max, int l_max)
    - 4. set_speed(unsigned int speed)

    1. void assign_motor(int total_steps, int A_plus, int A_minus, int B_plus, int B_minus)
    - Assign `Rotate_motor` instance using arguments.
    - `Rotate_motor` has `Stepper` member (<Stepper.h>), which need total steps of motor and pins.
    - For `Rotate_button_bundle`, only stepper motors that have 4 digital pins are compatible.

    2. void assign_sensor(int sensor_port)
    - Assign `Sensor` instance with `sensor_port`.
    - The `Sensor` instance will recognize the input signal of `sensor_port`.
    - However, `Sensor` instance only alert when signal turns HIGH to LOW. (like push button)
    - So if your sensor has only On/Off switch, it will recognize when it's switch On to Off (On --> OFF)

    3. void assign_angle(int current_angle, int r_max, int l_max)
    - Assign maximum and initial (current) angles of stepper motor.
    - The stepper motor only rotates in angle between [l_max ~ r_max].
    - `l_max` will be set automatically negative, and `r_max` to positive.
    - The `current_angle` will treat stepper motor were posed at `current_angle`.
    - The angle of stepper motor is like down below.

                        [Front]
                            0
                            |
                            |
                            |
                            |
                            |
    [LEFT] -90 --------------------------- +90 [RIGHT]

    4. void set_speed(unsigned int speed)
    - Set speed of stepper motor.
    - If `speed` is too large, it can cause some issue.

    Receive and publish signals
    --------------------------------------------------------------------------------------------------------
    After initialize, you can use `process_signal` method to receive signals of `Sensor`, and rotate stepper motor.

    --> int process_signal() 
    - Check whether sinal has been received.
    - If it received signal, switch internal mode. (Spin --> Stop OR Stop --> Spin)
    - By "pushing" `Sensor`, stepper motor will stop or spin.

    - NOTICE
    If either `speed` OR `total_steps` were NOT initialized, `process_signal` method WILL BE ABORTED.
    If instance were not initialized, `process_signal` will only returns 1 and not switch internal mode.
    If not, returns 0 and switch internal mode.
    --------------------------------------------------------------------------------------------------------

    Dependency  :   <Stepper.h>
*/

Sensor_LED_bundle LED_bundle(motor, true, false);

Sensor_Stepper_bundle Step_bundle;

int total_step = 200;
int current_angle = 0;
int r_max = 60;
int l_max = -60;
unsigned int speed = 5;

// void setup()
// {
//     LED_bundle.assign_led(led_1);       // led_1 : 2
//     LED_bundle.assign_led(led_2);       // led_2 : 3
//     LED_bundle.assign_led(led_3);       // led_3 : 4
//     LED_bundle.assign_sensor(button_1); // button_1 : 5
//     // Done

//     Step_bundle.assign_motor(total_step, A_plus, A_minus, B_plus, B_minus);
//     Step_bundle.assign_sensor(button_2);                    // button_2 : 7
//     Step_bundle.assign_angle(current_angle, r_max, l_max);
//     Step_bundle.set_speed(speed);
// }

// void loop()
// {
//     int var1 = LED_bundle.process_signal();
//     int var2 = Step_bundle.process_signal();
    
//     // must delay few milisecond
//     // if not, LED_bundle will recognize Arduino's cluck signal as input
//     delay(100);
// }
