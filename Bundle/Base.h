
# ifndef __BASE__
# define __BASE__ "IMPLEMENTED"

# include <Arduino.h>

class Basic_port
{
    protected : 
        int Port;

        void output_setup()         {return pinMode(Port, OUTPUT);}
        void input_setup()          {return pinMode(Port, INPUT);}
        void input_pullup_setup()   {return pinMode(Port, INPUT_PULLUP);}

        void pub_digital(int val)   {return digitalWrite(Port, val);}
        void pub_analog(int val)    {return analogWrite(Port, val);}
        int read_digital()          {return digitalRead(Port);}
        int read_analog()           {return analogRead(Port);}

        void print_port()           {Serial.println(Port);}
    
    public : 
        Basic_port()            {Port = -1;}
        Basic_port(int port)    {Port = port;}
};

class LED : public Basic_port
{
    private : 
        void setup()                {return output_setup();}
    
    public :
        LED() : Basic_port() {}
        LED(int led_port) : Basic_port(led_port)    {output_setup();}
        ~LED()                                      {pub_digital(LOW);}
        
        void assign_port(int port)
        {
            Port = port;
            setup();
        }
        void pub_low()              {return pub_digital(LOW);}
        void pub_high()             {return pub_digital(HIGH);}
        void print_port()           {return Basic_port::print_port();}
};

class Sensor : public Basic_port
{
    protected :
        int Prev_status;

        void setup()                {return input_setup();}
        int read_signal()           {return read_digital();}
    
    public :
        Sensor() : Basic_port() {}
        Sensor(int sensor_port) : Basic_port(sensor_port)
        {
            Prev_status = LOW;
            setup();
        }

        void assign_port(int port)
        {
            Port = port;
            setup();
        }
        bool is_received()
        {
            int signal = read_signal();

            if ((signal != Prev_status) && (signal == LOW))
            {
                Prev_status = LOW;
                return true;
            }

            if (signal == HIGH)
            {
                Prev_status = HIGH;
            }

            return false;
        }
        
        void print_port()           {return Basic_port::print_port();}
};

# endif
