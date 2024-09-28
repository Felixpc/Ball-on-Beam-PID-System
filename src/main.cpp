#include <Arduino.h>
#include "ultrasound.h"
#include <Servo.h>
#include "LowPass.h"
#include "PID.h"
#include "Settings.h"

unsigned long timing=0;

unsigned loop_interval_time=0;
int loopcounter=0;

float Pgain=15.0f;
float Igain=0.08f;
float Dgain=55.0f;//45
float setpoint=14.0;

PID pid=PID();

LowPassFilter lowpass=LowPassFilter(20, 0.04f);

Ultrasound sensor = Ultrasound(3,2);

Settings settings = Settings();

Servo servo;
void setup() {
    Serial.begin(9600);

    servo.attach(5);

    settings.init();

    settings.addValue(valuesetting("P gain setting", Pgain, 50));
    settings.addValue(valuesetting("I gain setting", Igain, 0.50));
    settings.addValue(valuesetting("D gain setting", Dgain, 100));
    settings.addValue(valuesetting("setpoint setting", setpoint, 16));
    settings.addValue(valuesetting("live setpoint setting", setpoint, 16, true));

}

float midpoint=1500;
float Servo_limit=500;

float distance=0;
float distance_old=0;
void loop() {
    settings.update();
    
    if(timing < micros()){
        timing = micros()+8700;

        distance -= (distance-sensor.measure_distance())*0.15;

        if(abs(distance-distance_old)>4.0){
            //distance=distance_old;
            //Serial.println("new measurement droopped");
        }

        if(distance > 40){
            distance = 40;
            pid.out=-100;
        }else{
            pid.calculatePID(distance, setpoint, &Pgain, &Igain, &Dgain);
        }
        //Serial.println(distance);
    
        float regelwert=pid.out;
        if(regelwert > Servo_limit){
            regelwert = Servo_limit;
        }
        if(regelwert < -Servo_limit){
            regelwert = -Servo_limit;
        }
        lowpass.update(midpoint+regelwert);
        servo.writeMicroseconds(lowpass.getOutput());
        distance_old=distance;

        loopcounter++;
        loop_interval_time = (micros()-(timing-8700));
        if((loopcounter & 100) ==0){
            //Serial.println(loop_interval_time);
            loopcounter=0;
        }
    } 
}


