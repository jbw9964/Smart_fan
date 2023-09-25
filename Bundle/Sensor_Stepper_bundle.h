
# ifndef __Sensor_Stepper_bundle__
# define __Sensor_Stepper_bundle__ "IMPLEMENTED"

# include <Arduino.h>
# include <Stepper.h>
# include "./Base.h"
# include "./Sensor_LED_bundle.h"

# define UNIT_STEP 1

class Stepper_motor
{
    private :
        int Pin_A, Pin_A_, Pin_B, Pin_B_;
        int Speed;

        Stepper *Step_motor;
    
    public :
        Stepper_motor();
        ~Stepper_motor();

        int Total_steps;

        void assign(int total_steps, int A_plus, int A_minus, int B_plus, int B_minus);
        void assign_steps(int total_steps);
        void assign_ports(int A_plus, int A_minus, int B_plus, int B_minus);
        void set_speed(unsigned int speed);
        void rotate_right(int step);
        void rotate_left(int step);
};
class Sensor_Stepper_bundle
{
    private :
        Stepper_motor Motor;
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

        int Current_angle;              // default : 0
        int R_max_angle;                // + value
        int L_max_angle;                // - value

        int steps_in_range;
        float current_step;             // must recognize type conversion
        unsigned int speed;

        bool Mode;                      // default : Stop
        bool direction;                 // default : left

        bool is_good();

        int step_to_angle(int step);
        int angle_to_step(int angle);

    public :
        Sensor_Stepper_bundle();
        ~Sensor_Stepper_bundle();

        void assign_motor(int total_steps, int A_plus, int A_minus, int B_plus, int B_minus);
        void assign_sensor(int sensor_port);
        void assign_angle(int current_angle, int r_max, int l_max);
        void reset_max_angle(int r_max, int l_max);
        void set_speed(unsigned int speed);

        int process_signal();
        int process_signal(Sensor_LED_bundle &led_bundle);
};

# ifndef __Sensor_Stepper_bundle_func__
# include "Sensor_Stepper_bundle.cpp"
# endif

# endif