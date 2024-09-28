#include "Arduino.h"
#include "Input.h"

typedef struct valuesetting{
    valuesetting(String name, float &value, float step){
        this->name=name;
        this->value=&value;
        this->step=step;
    }
    valuesetting(){

    }
    String name;
    float *value;
    float step=1;

} valuesetting_t;

Input input = Input(A0, 4);


float currentmodifier=0;

valuesetting_t set_names[10];

typedef enum {
    P_setting=0,
    I_setting=1,
    D_setting=2,
    Setpoint_setting=3
} modes_t;
        
        modes_t current_mode=P_setting;


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
                Serial.println("current mode: " + set_names[current_mode].name);
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
            currentmodifier=input.getNormalizedPosition()*set_names[current_mode].step;
            
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