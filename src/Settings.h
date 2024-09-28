#include "Arduino.h"
#include "Input.h"


#include <U8x8lib.h>


U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ A5, /* data=*/ A4, /* reset=*/ U8X8_PIN_NONE);


typedef struct valuesetting{
    valuesetting(String name, float &value, float max){
        this->name=name;
        this->value=&value;
        this->max=max;
    }
    valuesetting(String name, float &value, float max, bool livesetting){
        this->name=name;
        this->value=&value;
        this->max=max;
        this->livesetting=livesetting;
    }
    valuesetting(){

    }
    String name;
    float *value;
    float max=1;
    bool livesetting=false;

} valuesetting_t;

Input input = Input(A0, 4);


float currentmodifier=0;

valuesetting_t set_names[5];


        
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
        u8x8.begin();

        u8x8.setFont(u8x8_font_px437wyse700a_2x2_r);
        u8x8.drawString(0, 0, "PID CTRL");

        u8x8.setFont(u8x8_font_px437wyse700a_2x2_r);
        u8x8.drawString(0, 3, "SETTINGS");
  
        u8x8.refreshDisplay();
    }

    void update(){
        input.update();
        if(timing < millis()){
            timing = millis()+100;

        if(!In_selectionmode){
            currentmodifier=input.getNormalizedPosition()*set_names[current_mode].max;
            if(set_names[current_mode].livesetting){
                *set_names[current_mode].value-=(*set_names[current_mode].value-currentmodifier)*0.1f;
            }
            
            Serial.print("selection: ");
            Serial.print(currentmodifier);
            Serial.print("             \r");
        }
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