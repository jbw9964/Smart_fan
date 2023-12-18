
# ifndef __Bundle_sensor_servo__
# define __Bundle_sensor_servo__

# include <Arduino.h>
# include <Servo.h>
# include <base/base.h>

# define MAX(a, b)  ((a) > (b) ? (a) : (b))
# define MIN(a, b)  ((a) < (b) ? (a) : (b))

enum Spin_direction
{
    Left = true,
    Right = false
};

enum Spin_mode
{
    Spin = true,
    Stop = false
};

class sensor_servo_bundle
{
    private:
        Servo *Servo_motor;
        Sensor *Input_sensor;

        int Port_output;
        int Port_input;

        int L_max_angle;
        int R_max_angle;
        int Current_angle;
        int Unit_step;

        bool Mode;
        bool Direction;

        bool is_servo_good;
        bool is_input_good;
        bool is_good();

    public:
        sensor_servo_bundle();
        ~sensor_servo_bundle();

        int mode;
        int angle_Current;
        int angle_Left;
        int angle_Right;

        void assign_input_sensor(int);
        void assign_servo(int);
        void assign_servo(int, int);
        void assign_servo(int, int, int);
        void assign_servo(int, int, int, int);

        void assign_angle(int, int);
        void assign_angle(int, int, bool);

        int process_signal();
        int process_signal(int);
        int process_signal(bool);
        int process_signal(int, bool);

        void print_port();
};


# ifndef __Bundle_sensor_servo_func__
    # include <sensor_servo_bundle/sensor_servo_bundle.cpp>
# endif

# endif