#pragma once
//TODO this class and global variables is not perfect
#include "Arduino.h"
#include "Input.h"

#define MAX_SETTINGS 5//TODO use dynamic approach

namespace SETTINGS{

typedef struct valuesetting{
    valuesetting(String name, String shortdescriptor, float &value, float max){
        this->name=name;
        this->value=&value;
        this->max=max;
        this->shortdescriptor=shortdescriptor;
    }
    valuesetting(String name, String shortdescriptor, float &value, float max, bool livesetting){
        this->name=name;
        this->value=&value;
        this->max=max;
        this->livesetting=livesetting;
        this->shortdescriptor=shortdescriptor;
    }
    valuesetting(){};

    String name;
    float *value;
    float max=1;
    bool livesetting=false;
    String shortdescriptor;

} valuesetting_t;

valuesetting_t set_names[MAX_SETTINGS];

Input input = Input(A0, 4);

float currentmodifier=0;

int current_mode=0;

bool In_selectionmode=true;

int valuecount=0;

void ( *changeCallback )(int) = nullptr;

class Settings{
    public:
    void init(){
        Serial.println("############   Ball on Beam PID System               #########");
        Serial.println("############   Please Select a value to manipulate   #########");

        input.setup();

        input.registerPotiStepCallback([](int pos){
            if(In_selectionmode){
                current_mode=pos;
                Serial.print("current mode: " + set_names[current_mode].name + "                \r");
                if(changeCallback!=nullptr){
                    changeCallback(current_mode);
                }
            }else{
                return;//prevents timing problem
            }

        });

        input.registerButtonPushbackCallback([](){
            In_selectionmode=!In_selectionmode;
            if(!In_selectionmode){
                Serial.println("############   Mode Selected: " + set_names[current_mode].name);
                Serial.print("current value: " + set_names[current_mode].name + " = ");
                Serial.println(*set_names[current_mode].value);
                currentmodifier=0;
                if(changeCallback!=nullptr){
                    changeCallback(current_mode);
                }
            }
            if(In_selectionmode){
                Serial.println("############   Please Select a value to manipulate");
                Serial.print("Applied ");
                Serial.print(currentmodifier);
                Serial.print(" to ");
                Serial.println(set_names[current_mode].name);
                *set_names[current_mode].value = currentmodifier;
                if(changeCallback!=nullptr){
                    changeCallback(current_mode);
                }

            }
        });
    }
    int timingcounter=0;
    void update(){
        input.update();
        if(timing < millis()){
            timing = millis()+100;

        if(!In_selectionmode){
            currentmodifier=input.getNormalizedPosition()*set_names[current_mode].max;
            if(set_names[current_mode].livesetting){
                *set_names[current_mode].value-=(*set_names[current_mode].value-currentmodifier)*0.1f;
            }else{
                if(changeCallback!=nullptr && timingcounter>8){
                    changeCallback(current_mode);
                    timingcounter=0;
                }
            }
            
            Serial.print("selection: ");
            Serial.print(currentmodifier);
            Serial.print("             \r");

        }
            timingcounter++;
        }
    }



    void addValue(valuesetting_t new_value_setting){
        set_names[valuecount] = new_value_setting;
        valuecount++;
        input.setStepCount(valuecount);
    }
    private:
        float poti_old;
        unsigned long timing;

};

}