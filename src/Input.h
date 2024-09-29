#include "Arduino.h"
#include <Wire.h>

//#include <U8g2lib.h>



class Input{

public:
Input(int pin_poti, int pin_button){
    this->pin_poti=pin_poti;
    this->pin_button=pin_button;

    pinMode(this->pin_poti, INPUT);
    pinMode(this->pin_button, INPUT);


}
void setup(){
    //u8g2.setI2CAddress(0x3C);

   //u8g2.begin();

}
void update(){
    if(timing < micros()){
        timing=micros()+1000;


        int pos=0;
        for(int i=0;i<allsteps;i++){
            if((analogRead(pin_poti)/1023.0)>=(i*(1.0/allsteps)))
            {
                pos=i;
            }
        }
        PotiPos=pos;
        if(PotiPos!=PotiPos_old){
            if(poticallback!=nullptr){
                poticallback(PotiPos);
            }
        }
        PotiPos_old=PotiPos;

        if(digitalRead(pin_button)){
            buttoncounter++;
        }else{
            buttoncounter--;
        }
        

        if(buttoncounter>=10){
            buttoncounter=10;
            button_state=false;
            //Serial.println("button pressed");
        }
        if(buttoncounter<=-10){
            buttoncounter=-10;
            button_state=true;
            //Serial.println("button released");
        }
        if((old_button_state!=button_state) && (button_state==true)){
            if(buttoncallback!=nullptr){
                buttoncallback();
            }
        }
        old_button_state=button_state;

    }

}

void registerPotiCallback(void (*callback)(int)){
    this->poticallback=callback;
}

void registerButtonPushbackCallback(void (*callback)()){
    this->buttoncallback=callback;
}

float getNormalizedPosition(){
    return analogRead(pin_poti)/1023.0;
}

void setStepCount(int steps){
    this->allsteps=steps;
}

private:  
int pin_poti;
int pin_button;
unsigned long timing=0;

void ( *poticallback )( int ) = nullptr;
void ( *buttoncallback )() = nullptr;

int allsteps=4;

int PotiPos=0;
int PotiPos_old=0;

bool button_state=true;
bool old_button_state=true;

int buttoncounter=0;

};