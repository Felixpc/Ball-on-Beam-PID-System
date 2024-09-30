#include "Arduino.h"


class Input{

public:
Input(int pin_poti, int pin_button){
    this->pin_poti=pin_poti;
    this->pin_button=pin_button;

    pinMode(this->pin_poti, INPUT);
    pinMode(this->pin_button, INPUT);
}
void setup(){


}
void update(){
    if(timing < micros()){
        timing=micros()+1000;

        //dividing poti into multiple steps for selection
        int pos=0;
        static float spacing=0;//prevents flickering at transitions
        for(int i=0;i<stepCount;i++){
            if((analogRead(pin_poti)/1023.0)>=(i*(1.0/stepCount))-spacing)
            {
                pos=i;
            }else{
                //has to be testet
                if(pos>PotiPos)
                    spacing=0.01;
                if(pos<PotiPos)
                    spacing=-0.01;
                    break;

            
            }
        }
        PotiPos=pos;
        if(PotiPos!=PotiPos_old){
            if(potiStepCallback!=nullptr){
                potiStepCallback(PotiPos);//call on change callback
            }
        }
        PotiPos_old=PotiPos;
        //simple debouncing of push button
        if(digitalRead(pin_button)){
            debounceCounter++;
        }else{
            debounceCounter--;
        }
        
        if(debounceCounter>=10){
            debounceCounter=10;
            button_state=false;
        }
        if(debounceCounter<=-10){
            debounceCounter=-10;
            button_state=true;
        }
        if((old_button_state!=button_state) && (button_state==true)){//detect pushback and call callback
            if(buttoncallback!=nullptr){
                buttoncallback();
            }
        }
        old_button_state=button_state;
    }
}

void registerPotiStepCallback(void (*callback)(int)){
    this->potiStepCallback=callback;
}

void registerButtonPushbackCallback(void (*callback)()){
    this->buttoncallback=callback;
}

float getNormalizedPosition(){
    return analogRead(pin_poti)/1023.0;
}

void setStepCount(int steps){
    this->stepCount=steps;
}

private:
//pins
int pin_poti;
int pin_button;

unsigned long timing=0;

int stepCount=4;

int PotiPos=0;
int PotiPos_old=0;

bool button_state=true;
bool old_button_state=true;

int debounceCounter=0;

//callbacks
void ( *potiStepCallback )( int ) = nullptr;
void ( *buttoncallback )() = nullptr;

};