#ifndef ULTRASOUND_H
#define ULTRASOUND_H
#include <Arduino.h>


class Ultrasound{
    public:

        Ultrasound(int pin_trigger, int pin_echo){
            this->pin_trigger=pin_trigger;
            this->pin_echo=pin_echo;

            pinMode(this->pin_trigger, OUTPUT);
            pinMode(this->pin_echo, INPUT);

            digitalWrite(this->pin_trigger, LOW);
        }

        unsigned long measure_pulselength(){
            digitalWrite(this->pin_trigger, LOW);
            delayMicroseconds(10);
            digitalWrite(this->pin_trigger, HIGH);
            delayMicroseconds(10);
            digitalWrite(this->pin_trigger, LOW);

            return pulseIn(pin_echo, HIGH);
        }
        float measure_distance(){
            return measure_pulselength()/29.0f/2.0f;
        }

    private:
        int pin_trigger;
        int pin_echo;

};

#endif ULTRASOUND_H