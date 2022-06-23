
#include "servo.h"

servo::servo(int pin)
{
    myservo = new Servo;

    myservo->setPeriodHertz(50); // standard 50 hz servo
    myservo->attach(pin, 1000, 2000);
}
servo::~servo()
{
    delete myservo;
}

void servo::open()
{
    for (pos = 0; pos <= 180; pos += 1)
    { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo->write(pos); // tell servo to go to position in variable 'pos'
        delay(7);            // waits 15ms for the servo to reach the position
    }
}
void servo::close()
{
    for (pos = 180; pos >= 0; pos -= 1)
    {                        // goes from 180 degrees to 0 degrees
        myservo->write(pos); // tell servo to go to position in variable 'pos'
        delay(7);            // waits 15ms for the servo to reach the position
    }
}
void servo::sweep()
{
    for (pos = 0; pos <= 180; pos += 1)
    { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo->write(pos); // tell servo to go to position in variable 'pos'
        delay(7);            // waits 15ms for the servo to reach the position
    }

    for (pos = 180; pos >= 0; pos -= 1)
    {                        // goes from 180 degrees to 0 degrees
        myservo->write(pos); // tell servo to go to position in variable 'pos'
        delay(7);            // waits 15ms for the servo to reach the position
    }
}

void servo::set_speed(int desired_speed)
{
    vel = desired_speed;
}
