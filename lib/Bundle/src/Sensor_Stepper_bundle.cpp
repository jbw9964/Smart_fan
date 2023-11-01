
# ifndef __Sensor_Stepper_bundle_func__
# define __Sensor_Stepper_bundle_func__

# ifndef __Sensor_Stepper_bundle__
    # include "./Sensor_Stepper_bundle.h"
# endif

Stepper_motor::Stepper_motor()
{
    Pin_A = -1;
    Pin_A_ = -1;
    Pin_B = -1;
    Pin_B_ = -1;

    Step_motor = nullptr;
}
Stepper_motor::~Stepper_motor()
{
    delete Step_motor;
    Step_motor = nullptr;
}
void Stepper_motor::assign(int total_steps, int A_plus, int A_minus, int B_plus, int B_minus)
{
    assign_steps(total_steps);
    assign_ports(A_plus, A_minus, B_plus, B_minus);
    Step_motor = new Stepper(Total_steps, Pin_A, Pin_A_, Pin_B, Pin_B_);
}
void Stepper_motor::assign_steps(int total_steps)
{
    Total_steps = total_steps;
}
void Stepper_motor::assign_ports(int A_plus, int A_minus, int B_plus, int B_minus)
{
    Pin_A = A_plus;
    Pin_A_ = A_minus;
    Pin_B = B_plus;
    Pin_B_ = B_minus;
}
void Stepper_motor::set_speed(unsigned int speed)
{
    Speed = speed;
    Step_motor->setSpeed(Speed);
}
void Stepper_motor::rotate_right(int step)
{
    return Step_motor->step(step);
}
void Stepper_motor::rotate_left(int step)
{
    return Step_motor->step(-1 * abs(step));
}


bool Sensor_Stepper_bundle::is_good()
{
    bool flag = true;
    if (!speed)                     {flag = false;}
    if (!Motor.Total_steps)         {flag = false;}
    if (!(Motor.Total_steps / 2))   {flag = false;}

    return flag;
}
int Sensor_Stepper_bundle::step_to_angle(int step)
{
    int half_step = Motor.Total_steps / 2;
    if (!Motor.Total_steps || !half_step)       {return -360;}
    
    step = (step % Motor.Total_steps) - half_step / 2;
    step = step >= half_step ? step - Motor.Total_steps : step;
    // (step / half_step) = (angle / 180)

    return (float) (step) / half_step * 180;
}
int Sensor_Stepper_bundle::angle_to_step(int angle)
{
    int half_step = Motor.Total_steps / 2;
    if (!Motor.Total_steps || !half_step)       {return -1;}

    angle = (angle % 360) + 90;
    // (angle / 360) = (step / Total_step)

    return (int) (((float) (angle) / 360) * (float) (Motor.Total_steps));

}
Sensor_Stepper_bundle::Sensor_Stepper_bundle()
{
    Current_angle = 0;
    R_max_angle = 0;
    L_max_angle = 0;
    steps_in_range = 0;
    speed = 0;
    Mode = Stop;
    direction = Left;
}
Sensor_Stepper_bundle::~Sensor_Stepper_bundle()
{
    delete &Motor;
    delete &Power_sensor;
}
void Sensor_Stepper_bundle::assign_motor(int total_steps, int A_plus, int A_minus, int B_plus, int B_minus)
{
    return Motor.assign(total_steps, A_plus, A_minus, B_plus, B_minus);
}
void Sensor_Stepper_bundle::assign_sensor(int sensor_port)
{
    return Power_sensor.assign_port(sensor_port);
}
void Sensor_Stepper_bundle::assign_angle(int current_angle, int r_max, int l_max)
{
    Current_angle = current_angle;
    R_max_angle = abs(r_max);
    L_max_angle = -1 * abs(l_max);

    steps_in_range = (int) ((R_max_angle - L_max_angle) / 360 * Motor.Total_steps);
    current_step = angle_to_step(current_angle);
}
void Sensor_Stepper_bundle::reset_max_angle(int r_max, int l_max)
{
    return assign_angle(Current_angle, r_max, l_max);
}
void Sensor_Stepper_bundle::set_speed(unsigned int speed)
{
    this->speed = speed;
    return Motor.set_speed(this->speed);
}
int Sensor_Stepper_bundle::process_signal()
{
    if (!is_good())                     {return 1;}

    if (Power_sensor.is_received())     {Mode = !Mode;}

    if (Mode == Spin)
    {
        if (direction == Right)
        {
            Motor.rotate_right(UNIT_STEP);
            current_step += UNIT_STEP;
            Current_angle = step_to_angle(current_step);
        }
        else
        {
            Motor.rotate_left(UNIT_STEP);
            current_step -= UNIT_STEP;
            Current_angle = step_to_angle(current_step);
        }
        
        if (!(L_max_angle <= Current_angle && Current_angle <= R_max_angle))
        {
            direction = !direction;
        }
        else if (Current_angle >= R_max_angle && direction == Right)
        {
            direction = Left;
        }
        else if (Current_angle <= L_max_angle && direction == Left)
        {
            direction = Right;
        }
    }

    return 0;
}

int Sensor_Stepper_bundle::process_signal(Sensor_LED_bundle &led_bundle)
{
    int var = led_bundle.process_signal();
    if (var)    {return 1;}

    return this->process_signal();
}

# endif