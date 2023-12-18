
# ifndef __Sensor_Servo_bundle__
# define __Sensor_Servo_bundle__ "IMPLEMENTED"

# include <Arduino.h>
# include <Servo.h>
# include "./Base.h"

class Sensor_Servo_bundle
{
    private :
        Servo Servo_motor;
        Sensor Power_sensor;

        enum direction
        {
            Right = true,
            Left = false,
        };
        enum mode
        {
            Stop = true,
            Spin = false,
        };

        int Current_angle;              // 0 <= Current_angle <= 180
        int R_max_angle;                // default : 180
        int L_max_angle;                // default : 0
        int Unit_step;

        bool Mode;
        bool direction;

        bool is_good();
    
    public :
        Sensor_Servo_bundle();
        ~Sensor_Servo_bundle();

        void assign(int sensor_port, int motor_port, int unit_step);
        void assign_sensor(int sensor_port);
        int assign_motor(int motor_port, int unit_step);
        void assign_angle(int l_max, int r_max);

        int process_signal();
};

# ifndef __Sensor_Servo_bundle_func__
# include "Sensor_Servo_bundle.cpp"
# endif

# endif