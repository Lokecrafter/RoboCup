#ifndef PHOTORESISTOR_H
#define PHOTORESISTOR_H

#include"Vector.h"
#include"MathFuncs.h"

class PhotoResistor {
    private:
        int whiteVal;
        int blackVal;
    public:
        int pin;
        Vector2 position;
        void setWhiteBlackVals(int newWhiteVal, int newBlackVal){
            whiteVal = newWhiteVal;
            blackVal = newBlackVal;
        }
        //Default constructor
        PhotoResistor(){

        }

        float getLight(){
            float val = analogRead(pin);
            return floatMap(val, whiteVal, blackVal, 0, 1);
        }
};
#endif