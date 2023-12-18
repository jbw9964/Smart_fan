
// # include <Arduino.h>
// # include <bundle_refactor.h>
// # include <Raspberry_pi_Json.h>

// // # define SERIAL_SPEED   115200
// // # define KEY_MSG_1      "Angle_left"                    // integer between [0 ~ 180]
// // # define KEY_MSG_2      "Angle_right"                   // integer between [0 ~ 180]
// // # define KEY_MSG_3      "Is_detected"                   // boolean
// // # define KEY_MSG_4      "QUIT"                          // boolean

// enum LED_PORTS
// {
//     led1 = 2,
//     led2 = 3,
//     led3 = 4,
// };

// # define PORT_INPUT_SENSOR_1    5
// # define PORT_MOTOR             10

// # define PORT_INPUT_SENSOR_2    6
// # define PORT_SERVO             8
// # define L_max_angle            0
// # define R_max_angle            180
// # define UNIT_STEP              1

// sensor_LED_bundle sensor_led_bundle;
// sensor_servo_bundle sensor_servo;
// Raspberry_pi_Json serial_receiver(&Serial, &Serial);


// void setup()
// {
//     serial_receiver.begin_serial(9600);
    
//     sensor_led_bundle.assign_led(led1);
//     sensor_led_bundle.assign_led(led2);
//     sensor_led_bundle.assign_led(led3);
//     sensor_led_bundle.assign_input_sensor(PORT_INPUT_SENSOR_1);

//     sensor_led_bundle.setup_output(PORT_MOTOR, false);

//     sensor_servo.assign_input_sensor(PORT_INPUT_SENSOR_2);
//     sensor_servo.assign_servo(PORT_SERVO, L_max_angle, R_max_angle, UNIT_STEP);
//     sensor_servo.assign_angle(L_max_angle, R_max_angle, true);

//     Serial.println("Done setup\n");
//     sensor_led_bundle.print_port();
//     Serial.println("\n");
//     sensor_servo.print_port();
//     Serial.println("\n");
//     delay(1000);
    
//     Serial.println("Start\n");
// }

// void loop()
// {
//     int var1 = sensor_led_bundle.process_signal();

//     int var2;
//     if (sensor_led_bundle.level)
//     {
//         var2 = sensor_servo.process_signal();
//     }
    
//     // Serial.println(var1);
//     // Serial.println(var2);

//     if (sensor_led_bundle.level && serial_receiver.receive_msg())
//     {
//         if (serial_receiver.Quit)
//         {
//             Serial.println("Exiting program...\n");
//             delay(100);
//             exit(0);
//         }

//         int prev_l = sensor_servo.angle_Left;
//         int prev_r = sensor_servo.angle_Right;

//         int angle_l = serial_receiver.angle_Left;
//         int angle_r = serial_receiver.angle_Right;
//         sensor_servo.assign_angle(angle_l, angle_r);

//         bool detection = serial_receiver.detection_Flag;

//         Serial.println("angle has been changed.");
//         Serial.print("[Left]  : ");
//         Serial.print(prev_l);
//         Serial.print(" --> ");
//         Serial.println(sensor_servo.angle_Left);

//         Serial.print("[Right] : ");
//         Serial.print(prev_r);
//         Serial.print(" --> ");
//         Serial.println(sensor_servo.angle_Right);

//         Serial.print("[Detection] : ");
//         Serial.println(detection ? "true" : "false");

//         Serial.println("\n");
//     }
    
//     delay(100);
// }

// /*
// sample json data
// {'Angle_left': 60, 'Angle_right': 120, 'Is_detected': true}
// {'Angle_left': 45, 'Angle_right': 135, 'Is_detected': false}
// {'Angle_left': 30, 'Angle_right': 150, 'Is_detected': true}
// {'Angle_left': 15, 'Angle_right': 165, 'Is_detected': false}
// {'Angle_left': 0, 'Angle_right': 180, 'Is_detected': true}
// */
