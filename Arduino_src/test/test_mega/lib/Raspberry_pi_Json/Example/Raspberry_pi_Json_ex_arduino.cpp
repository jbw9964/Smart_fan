
# include <Arduino.h>
# include <Raspberry_pi_Json.h>

Raspberry_pi_Json receiver;

void setup()
{
    receiver.begin_serial();

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Serial.println("Done setup\n");
    delay(100);
}


int angle_l = 0;
int angle_r = 180;
bool is_detected = false;

void loop()
{
    if (receiver.receive_msg())
    {
        if (receiver.Quit)
        {
            Serial.println("Exiting program...\n");
            delay(100);
            exit(0);
        }
        angle_l = receiver.angle_Left;
        angle_r = receiver.angle_Right;
        is_detected = receiver.detection_Flag;

        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);
    }

    Serial.print("[angle left] : ");
    Serial.println(angle_l);
    Serial.print("[angle right] : ");
    Serial.println(angle_r);
    Serial.print("[Is_detected] : ");
    Serial.println(is_detected);
    Serial.println("");
    delay(1000);
}

/*
sample json data
{'Angle_left': 60, 'Angle_right': 120, 'Is_detected': true}
{'Angle_left': 45, 'Angle_right': 135, 'Is_detected': false}
{'Angle_left': 30, 'Angle_right': 150, 'Is_detected': true}
{'Angle_left': 15, 'Angle_right': 165, 'Is_detected': false}
{'Angle_left': 0, 'Angle_right': 180, 'Is_detected': true}
*/