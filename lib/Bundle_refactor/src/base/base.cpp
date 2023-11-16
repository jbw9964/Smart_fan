
# ifndef __Bundle_base_func__
# define __Bundle_base_func__

# include <base/base.h>

/*
Basic_port:
    protected:
        int Port;

        void output_setup();
        void input_setup();
        void input_pullup_setup();

        void write_digital(int);
        void write_analog(int);
        int read_digital();
        int read_analog();

        void print_port();
    
    public:
        Basic_port();
        Basic_port(int);
*/
Basic_port::Basic_port()                {Port = -1;}                        // set Port as -1 (invalid port)
Basic_port::Basic_port(int port)        {Port = port;}                      // set Port
void Basic_port::output_setup()         {pinMode(Port, OUTPUT);}            // set Port's pinMode as OUTPUT
void Basic_port::input_setup()          {pinMode(Port, INPUT);}             // set Port's pinMode as INTPUT
void Basic_port::input_pullup_setup()   {pinMode(Port, INPUT_PULLUP);}      // set Port's pinMode as INPUT_PULLUP
void Basic_port::write_digital(int val) {return digitalWrite(Port, val);}   // write HIGH to Port with digital
void Basic_port::write_analog(int val)  {return analogWrite(Port, val);}    // write value to Port with analog
int Basic_port::read_digital()          {return digitalRead(Port);}         // read Port's value (HIGH/LOW) and return
int Basic_port::read_analog()           {return analogRead(Port);}          // read Port's value and return
void Basic_port::print_port()           {if (Serial)    {Serial.println(Port);}}


/*
LED <- Basic_port:
    private:
        void setup();

    public:
        LED();
        LED(int);
        ~LED();

        void assign_port(int);
        void write_low();
        void write_high();
        void print_port();
*/
LED::LED() : Basic_port()                       {}                          // set Port as -1 (invalid port)
LED::LED(int led_port) : Basic_port(led_port)   {setup();}                  // set Port & setup pinMode
LED::~LED()                                     {write_low();}              // write LOW to Port & turn off
void LED::setup()                               {output_setup();}           // setup pinMode as OUTPUT
void LED::assign_port(int led_port)                                         // set Port & setup pinMode
{
    Port = led_port;
    setup();
}
void LED::write_low()                           {write_digital(LOW);}       // write LOW as digital 
void LED::write_high()                          {write_digital(HIGH);}      // write HIGH as digital
void LED::print_port()                          {Basic_port::print_port();}


/*
Sensor <- Basic_port:
    private:
        int Prev_status;

        void setup();
        int read();

    public:
        Sensor();
        Sensor(int);

        void assign_port(int);
        bool is_received();

        void print_port();
*/
Sensor::Sensor() : Basic_port()                             {Prev_status = LOW;}        // set Port as -1 (invalid port)
Sensor::Sensor(int sensor_port) : Basic_port(sensor_port)                               // set Port & set initial status as LOW & setup pinMode
{
    Prev_status = LOW;
    setup();
}
void Sensor::setup()                                        {input_setup();}            // setup pinMode as INPUT
int Sensor::read()                                          {return read_digital();}    // set Port & set initial status as LOW & setup pinMode
void Sensor::assign_port(int sensor_port)
{
    Port = sensor_port;
    Prev_status = LOW;
    setup();
}
bool Sensor::is_received()                                                              // receive signal from Port & if signal was changed from HIGH to LOW, return true
{
    int signal = read();

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
void Sensor::print_port()                                   {Basic_port::print_port();}

# endif