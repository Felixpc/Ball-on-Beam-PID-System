#include "math.h"

class PID{
public:
    PID(){

    }
    float out;

    /**
     * calculaes control value
     */
    float calculatePID(float soll, float ist, float *p_gain, float *i_gain, float *d_gain){
        float error = soll - ist;       //calculate error e(t)
        float P = error * *p_gain;      //calculate proportional term

        float D=(error-old_error) * *d_gain;//calculate differential term

        I_teil+=*i_gain*error;          //calculate integral term
        
        //integral clamping to prevent overshooting
        if(I_teil > I_limit){
            I_teil = I_limit;
        }
        if(I_teil < -I_limit){
            I_teil = -I_limit;
        }

        float I=I_teil;                 //apply I term

        

        //rolling average filter
        errorStorage[cpos]=error;
        cpos++;
        if(cpos >= bufferlength){
            cpos=0;
        }
        //calculate average of the last errors
        float sum=0;
        for(int i=0;i<bufferlength;i++){
            sum += errorStorage[i];
        }
        this->old_error=sum/bufferlength;

        this->out=P+I+D;//add everything together

        return out;
    }
    /**
     * resets all runtime variables of the PID controller
     */
    void reset(){
        this->I_teil=0;
        this->old_error=0;
        for(int i=0;i<bufferlength;i++){
            errorStorage[i]=0;
        }
    }
private:


    float old_error;
    float I_teil;

    float I_limit=50;


    static const int bufferlength=35; //depth of rolling average filter
    int cpos=0; //index of average array
    float errorStorage[bufferlength];

};