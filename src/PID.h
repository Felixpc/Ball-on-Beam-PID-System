#include "math.h"

class PID{
public:
    PID(){

    }

    float calculatePID(float soll, float ist, float *p_gain, float *i_gain, float *d_gain){
        float error = soll - ist;
        float P = error * *p_gain;

        float D=(error-old_error) * *d_gain;

        I_teil+=*i_gain*error;
        
        if(I_teil > I_limit){
            I_teil = I_limit;
        }
        if(I_teil < -I_limit){
            I_teil = -I_limit;
        }

        float I=I_teil;

        
        this->out=P+I+D;


        lastD[cpos]=error;
        cpos++;
        if(cpos >= bufferlength){
            cpos=0;
        }

        float sum=0;
        for(int i=0;i<bufferlength;i++){
            sum += lastD[i];
        }
        this->old_error=sum/bufferlength;

        return out;
    }
        float out;

private:


    float old_error;
    float I_teil;

    float I_limit=50;


    static const int bufferlength=35;
    int cpos=0;
    float lastD[bufferlength];

};