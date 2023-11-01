
# ifndef __Bundle_base__
# define __Bundle_base__

# include <Arduino.h>

class Basic_port
{
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
};

class LED : public Basic_port
{
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
};

class Sensor : public Basic_port
{
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
};


# ifndef __Bundle_base_func__
    # include "./base.cpp"
# endif

# endif