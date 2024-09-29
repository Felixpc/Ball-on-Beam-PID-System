#pragma once

#include <U8x8lib.h>

class UI{

public:
    UI(int pin_SCL, int pin_SDA){
        u8x8=new U8X8_SSD1306_128X64_NONAME_SW_I2C(/* clock=*/ pin_SCL, /* data=*/ pin_SDA, /* reset=*/ U8X8_PIN_NONE);
    }
    void init(){
        u8x8->begin();

        u8x8->setFont(u8x8_font_px437wyse700a_2x2_r);
        u8x8->drawString(0, 0, "PID CTRL");

        u8x8->setFont(u8x8_font_px437wyse700a_2x2_r);
        u8x8->drawString(0, 3, "SETTINGS");

        u8x8->refreshDisplay();
    }
    unsigned long timing;
    void show(int line=-1){
        if(line>=4){//TODO scrolling
            //return;
        }
        if(SETTINGS::current_mode>=4){
            scrollposition=SETTINGS::current_mode-3;
        }else{
            scrollposition=0;
        }

        Serial.println("displayupdate");
        u8x8->setFont(u8x8_font_8x13_1x2_f);
        u8x8->setInverseFont(0);
        if(line==-1){
            u8x8->clear();
        }


        for(int i=scrollposition;i<4+scrollposition;i++){
            String data=": " + String(*SETTINGS::set_names[i].value);
            if(i==SETTINGS::current_mode){
                u8x8->setInverseFont(2);
                if(!SETTINGS::In_selectionmode){
                    u8x8->setFont(u8x8_font_8x13B_1x2_f);
                    data = ": " +String(SETTINGS::currentmodifier);
                }else{
                    u8x8->setFont(u8x8_font_8x13_1x2_f);
                }
            }else{
                u8x8->setInverseFont(0);
                u8x8->setFont(u8x8_font_8x13_1x2_f);
            }
            if(SETTINGS::set_names[i].livesetting){
                data="";
            }
            String toprint = (SETTINGS::set_names[i].shortdescriptor + data);
            u8x8->drawString(0, (i-scrollposition)*2, toprint.c_str());

            for(int x=toprint.length();x<16;x++){
                u8x8->drawString(x, (i-scrollposition)*2, " ");
            }
        }

    }
    void select(int selection){
        switch(selection){
            case 0:
                //u8x8->drawString(0, 0, ("P: " + String(p)).c_str());
            break;
        }
    }

private:
U8X8_SSD1306_128X64_NONAME_SW_I2C *u8x8;
int scrollposition=0;

};