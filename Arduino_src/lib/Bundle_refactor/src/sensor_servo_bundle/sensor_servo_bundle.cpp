
# ifndef __Bundle_sensor_servo_func__
# define __Bundle_sensor_servo_func__

# include <sensor_servo_bundle/sensor_servo_bundle.h>

/*
sensor_servo_bundle:
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

        enum mode
        {
            Spin = true,
            Stop = false
        };


    public:
        sensor_servo_bundle();
        ~sensor_servo_bundle();

        void assign_input_sensor(int);
        void assign_servo(int);
        void assign_servo(int, int, int);
        void assign_servo(int, int, int, int);

        void assign_angle(int, int);

        int process_signal();
        int process_signal(int);

        void print_port();
*/
sensor_servo_bundle::sensor_servo_bundle()
{
    Port_output = -1;
    Port_input = -1;
    L_max_angle = 0;
    R_max_angle = 180;
    Current_angle = 90;
    Unit_step = 1;

    Mode = Right;
    Mode = Stop;

    is_servo_good = false;
    is_input_good = false;

    Servo_motor = new Servo;
    Input_sensor = new Sensor;

    mode = Mode;
}
sensor_servo_bundle::~sensor_servo_bundle()
{
    delete Servo_motor;
    delete Input_sensor;
}
bool sensor_servo_bundle::is_good()
{
    return is_servo_good && is_input_good ? true : false;
}
void sensor_servo_bundle::assign_input_sensor(int sensor_port)
{
    Port_input = sensor_port;
    Input_sensor->assign_port(Port_input);
    
    is_input_good = true;
}
void sensor_servo_bundle::assign_servo(int servo_port)
{
    Port_output = servo_port;
    Servo_motor->attach(Port_output);
    
    is_servo_good = true;
}
void sensor_servo_bundle::assign_servo(int servo_port, int unit_step)
{
    Unit_step = unit_step;
    assign_servo(servo_port);
}
void sensor_servo_bundle::assign_servo(int servo_port, int angle_L, int angle_R)
{
    assign_angle(angle_L, angle_R);
    assign_servo(servo_port);
}
void sensor_servo_bundle::assign_servo(int servo_port, int angle_L, int angle_R, int unit_step)
{
    Unit_step = unit_step;
    assign_servo(servo_port, angle_L, angle_R);
}
void sensor_servo_bundle::assign_angle(int angle_L, int angle_R)
{
    if (angle_R < angle_L && Serial)
    {
        Serial.print("[Warning] : improper angle received - [L : ");
        Serial.print(angle_L);
        Serial.print(" | R : ");
        Serial.print(angle_R);
        Serial.println("]");
    }
    L_max_angle = MIN(angle_L, angle_R);
    R_max_angle = MAX(angle_L, angle_R);
    angle_Left = L_max_angle;
    angle_Rright = R_max_angle;
}
void sensor_servo_bundle::assign_angle(int angle_L, int angle_R, bool recognize_angle)
{
    if (recognize_angle)
    {
        Current_angle = Servo_motor->read();
        angle_Current = Current_angle;
    }
    assign_angle(angle_L, angle_R);
}
int sensor_servo_bundle::process_signal()
{
    if (!is_good())                     {return 1;}

    if (Input_sensor->is_received())    {Mode = !Mode;}

    if (Mode == Spin)
    {
        if (Current_angle <= L_max_angle)
        {
            Direction = Right;
        }
        else if (R_max_angle <= Current_angle)
        {
            Direction = Left;
        }

        Current_angle += (Direction == Right) ? Unit_step : -1 * Unit_step;

        Servo_motor->write(Current_angle);
    }

    mode = Mode;
    angle_Current = Current_angle;

    return 0;
}
int sensor_servo_bundle::process_signal(int prev_process)
{
    if (prev_process)
    {
        if (Serial)
        {
            Serial.println("[sensor_servo_bundle::process_signal] has been aborted, since previous process was corrupted.");
        }

        return 1;
    }

    return process_signal();
}
int sensor_servo_bundle::process_signal(bool recognize_angle)
{
    if (recognize_angle)
    {
        Current_angle = Servo_motor->read();
    }

    return process_signal();
}
int sensor_servo_bundle::process_signal(int prev_process, bool recognize_angle)
{
    if (prev_process)
    {
        if (Serial)
        {
            Serial.println("[sensor_servo_bundle::process_signal] has been aborted, since previous process was corrupted.");
        }

        return 1;
    }

    return process_signal(recognize_angle);
}
void sensor_servo_bundle::print_port()
{
    if (Serial)
    {
        Serial.print("[Servo] : ");
        Serial.println(Port_output);
        Serial.print("[Input sensor] : ");
        Input_sensor->print_port();
    }
}

# endif