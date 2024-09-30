#include <Arduino.h>
#include "Ultrasound.h"
#include <Servo.h>
#include "LowPass.h"
#include "PID.h"
#include "Settings.h"
#include "UI.h"

unsigned long timing=0;

unsigned loop_interval_time=0;
int loopcounter=0;

float midpoint=1330;//PWM pulse at level
float Servo_limit=500;

float distance=0;
float distance_old=0;

//controll gains preconfig
float Pgain=15.0f;
float Igain=0.08f;
float Dgain=55.0f;//45

float setpoint=14.0;

int offset=50;

PID pid=PID();
LowPassFilter lowpass=LowPassFilter(20, 0.04f);
Ultrasound sensor = Ultrasound(3,2);
SETTINGS::Settings settings = SETTINGS::Settings();
UI *ui = new UI(A5, A4);

Servo servo;
void setup() {
    Serial.begin(9600);

    servo.attach(5);
    servo.writeMicroseconds(midpoint-offset);
    ui->init();
    settings.init();

    settings.addValue(SETTINGS::valuesetting("P gain setting", "P", Pgain, 50));
    settings.addValue(SETTINGS::valuesetting("I gain setting", "I", Igain, 0.50));
    settings.addValue(SETTINGS::valuesetting("D gain setting", "D", Dgain, 100));
    settings.addValue(SETTINGS::valuesetting("setpoint setting", "SET", setpoint, 16));
    settings.addValue(SETTINGS::valuesetting("live setpoint setting", "Live Setpoint", setpoint, 16, true));

    SETTINGS::changeCallback=[](int line){
        ui->show(line);
    };

    ui->setBeforeShowCallback([](){
        servo.writeMicroseconds(midpoint-offset);
        pid.reset();
    });

    delay(3000);
    ui->show();
    pid.reset();
}

void loop() {
    settings.update();
    
    if(timing < micros()){
        timing = micros()+8700;

        distance -= (distance-sensor.measure_distance())*0.15;


        if(ui->isWorking() || (!SETTINGS::In_selectionmode && !SETTINGS::set_names[SETTINGS::current_mode].livesetting)){
            pid.reset();
            servo.writeMicroseconds(midpoint-offset);
            return;
        }

        if(distance > 40){ //recover from ball to close to sensor
            distance = 40;
            pid.reset();
            servo.writeMicroseconds(midpoint-offset);
            timing=micros()+2000000;
            return;
        }else{
            pid.calculatePID(distance, setpoint, &Pgain, &Igain, &Dgain);
        }
    
        float regelwert=pid.out;
        if(regelwert > Servo_limit){
            regelwert = Servo_limit;
        }
        if(regelwert < -Servo_limit){
            regelwert = -Servo_limit;
        }
        lowpass.update(midpoint+regelwert);//lowpass filter is not mandatory at this place
        servo.writeMicroseconds(lowpass.getOutput());
        distance_old=distance;

        loopcounter++;
        loop_interval_time = (micros()-(timing-8700));
        if((loopcounter & 100) == 0){
            //Serial.println(loop_interval_time);
            loopcounter=0;
        }
    } 
}


