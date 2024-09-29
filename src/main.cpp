#include <Arduino.h>
#include "ultrasound.h"
#include <Servo.h>
#include "LowPass.h"
#include "PID.h"
#include "Settings.h"
#include "UI.h"

unsigned long timing=0;

unsigned loop_interval_time=0;
int loopcounter=0;

float midpoint=1500;
float Servo_limit=500;

float distance=0;
float distance_old=0;


float Pgain=15.0f;
float Igain=0.08f;
float Dgain=55.0f;//45
float setpoint=14.0;

PID pid=PID();

LowPassFilter lowpass=LowPassFilter(20, 0.04f);

Ultrasound sensor = Ultrasound(3,2);

SETTINGS::Settings settings = SETTINGS::Settings();
UI *ui = new UI(A5, A4);

Servo servo;
void setup() {
    Serial.begin(9600);

    servo.attach(5);
    servo.writeMicroseconds(midpoint-50);
    ui->init();
    settings.init(nullptr);

    settings.addValue(SETTINGS::valuesetting("P gain setting", "P", Pgain, 50));
    settings.addValue(SETTINGS::valuesetting("I gain setting", "I", Igain, 0.50));
    settings.addValue(SETTINGS::valuesetting("D gain setting", "D", Dgain, 100));
    settings.addValue(SETTINGS::valuesetting("setpoint setting", "SET", setpoint, 16));
    settings.addValue(SETTINGS::valuesetting("live setpoint setting", "Live Setpoint", setpoint, 16, true));

    SETTINGS::changeCallback=[](int line){
        ui->show(line);
    };

    ui->setBeforeShowCallback([](){
        servo.writeMicroseconds(midpoint-50);
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
            servo.writeMicroseconds(midpoint-50);
            return;
        }


        if(abs(distance-distance_old)>4.0){
            //distance=distance_old;
            //Serial.println("new measurement droopped");
        }

        if(distance > 40){
            distance = 40;
            pid.reset();
            servo.writeMicroseconds(midpoint-50);
            timing=micros()+2000000;
            return;
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


