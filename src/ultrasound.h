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
        /**
         * triggers new measurement and returns travel time
         */
        unsigned long measure_pulselength(){
            //send trigger Pulse
            digitalWrite(this->pin_trigger, LOW);
            delayMicroseconds(10);
            digitalWrite(this->pin_trigger, HIGH);
            delayMicroseconds(10);
            digitalWrite(this->pin_trigger, LOW);

            //measure echo pulse length and return
            return pulseIn(pin_echo, HIGH, 5000);//timeout to prevent bloking through failed measurement
        }
        /**
         * triggers new measurement and returns distance in cm
         */
        float measure_distance(){
            return measure_pulselength()/29.0f/2.0f;//translate travel time to distance in cm
        }

    private:
        //pins
        int pin_trigger;
        int pin_echo;
};

#endif