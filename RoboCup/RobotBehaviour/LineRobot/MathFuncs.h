#ifndef MATHFUNCS_H
#define MATHFUNCS_H

//Global useful methods
float clamp(float val, float minVal, float maxVal){
    val = max(val, minVal);
    val = min(val, maxVal);
    return(val);
}

float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#endif