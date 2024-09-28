#ifndef PICODRONE_LOWPASSFILTER_H
#define PICODRONE_LOWPASSFILTER_H


class LowPassFilter{
public:
    //constructors
    LowPassFilter();
    LowPassFilter(float iCutOffFrequency, float iDeltaTime);
    //functions
    float update(float input);
    float update(float input, float deltaTime, float cutoffFrequency);
    //get and configure funtions
    float getOutput() const{return output;}
    void reconfigureFilter(float deltaTime, float cutoffFrequency);
private:
    float output;
    float ePow;
};


#endif //PICODRONE_LOWPASSFILTER_H