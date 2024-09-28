#include "Arduino.h"
#include "Input.h"

typedef struct valuesetting{
    valuesetting(String name, float &value, float max){
        this->name=name;
        this->value=&value;
        this->max=max;
    }
    valuesetting(){

    }
    String name;
    float *value;
    float max=1;

} valuesetting_t;

Input input = Input(A0, 4);


float currentmodifier=0;

valuesetting_t set_names[10];


        
int current_mode=0;


bool In_selectionmode=true;

int valuecount=0;

class Settings{
    public:
    void init(){
        Serial.println("### Welcome from the PID settings wizard ###");
        Serial.println("############   Please Select a value to manipulate");

        input.setup();

        input.registerPotiCallback([](int pos){
            if(In_selectionmode){
                current_mode=pos;
                Serial.print("current mode: " + set_names[current_mode].name + "                \r");
            }

        });

        input.registerButtonPushbackCallback([](){
            In_selectionmode=!In_selectionmode;
            if(!In_selectionmode){
                Serial.println("############   Mode Selected: " + set_names[current_mode].name);
                Serial.print("current value: " + set_names[current_mode].name + " = ");
                Serial.println(*set_names[current_mode].value);
                currentmodifier=0;
            }
            if(In_selectionmode){
                Serial.println("############   Please Select a value to manipulate");
                Serial.print("Applied ");
                Serial.print(currentmodifier);
                Serial.print(" to ");
                Serial.println(set_names[current_mode].name);
                *set_names[current_mode].value = currentmodifier;
            }
        });

    }

    void update(){
        input.update();
        if(timing < millis()){
            timing = millis()+100;

        if(!In_selectionmode){
            currentmodifier=input.getNormalizedPosition()*set_names[current_mode].max;
            
            Serial.print("selection: ");
            Serial.print(currentmodifier);
            Serial.print("             \r");
        }
        }
    }



    void addValue(valuesetting_t new_value_setting){
        set_names[valuecount] = new_value_setting;
        valuecount++;
    }
    private:
        float poti_old;
        unsigned long timing;

};