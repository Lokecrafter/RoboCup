#ifndef LINEFINDER_H
#define LINEFINDER_H

#include"Vector.h"
#include"PhotoResistor.h"

class LineFinder {
    private:
        PhotoResistor* photoResistors;
        int numPhotoResistors;

        float threshold = 0;
    public:
        //Setters
        void SetNumPhotoResistors(int newNum){numPhotoResistors = newNum;}
        void SetThreshold(float newThreshold){threshold = newThreshold;}
        void SetPhotoResistors(int* newPins, Vector2* newPositions, int* newWhiteVals, int* newBlackVals){
            delete [] photoResistors;
            photoResistors = new PhotoResistor[numPhotoResistors];

            for(int i = 0; i < numPhotoResistors; i++){
                photoResistors[i].pin = newPins[i];
                photoResistors[i].position = newPositions[i];
                photoResistors[i].setWhiteBlackVals(newWhiteVals[i], newBlackVals[i]);
            }
        }
        
        //numPins - amount of photoResistors
        //photoResistorPins - array of GPIO pins used by microcontroller
        //photoPositions - array of cartesian 2D-vectors defining a relative position to linefinder center
        //whiteValues - array of calibrated values for when linefinder sees white
        //blackValues - array of calibrated values for when linefinder sees black
        void begin(int numPins, int* photoResistorPins, Vector2* photoPositions, int* whiteValues, int* blackValues){
            SetNumPhotoResistors(numPins);
            SetPhotoResistors(photoResistorPins, photoPositions, whiteValues, blackValues);
        }
        //numPins - amount of photoResistors
        //photoResistorPins - array of GPIO pins used by microcontroller
        //photoAngles - array of polar 2D-vectors defining a relative position to linefinder center in terms of distance and degrees
        //whiteValues - array of calibrated values for when linefinder sees white
        //blackValues - array of calibrated values for when linefinder sees black
        void begin(int numPins, int* photoResistorPins, VectorP* photoAngles, int* whiteValues, int* blackValues){
            //Convert photoAngles from degrees to radians and then into 2D-vector
            Vector2 photoPositions[numPins];
            for(int i = 0; i < numPins; i++){
                photoAngles[i].angle *= PI/180;
                photoPositions[i] = (Vector2)photoAngles[i];
	        }
            SetNumPhotoResistors(numPins);
            SetPhotoResistors(photoResistorPins, photoPositions, whiteValues, blackValues);
        }
        ~LineFinder(){
            //Free memory and give back to OS
            delete [] photoResistors;
            photoResistors = nullptr;
        }

        VectorP getLine(){                                                           
            int index = 0;
            float maxVal = 0;
            //Find max value
            for(byte i = 0; i < numPhotoResistors; i++){
                float val = photoResistors[i].getLight();
                if(maxVal < val){
                    maxVal = val;
                    index = i;
                }
            }
            //Return the vector pointing straight forward if none of photo resistors are black enough
            if(maxVal <= threshold) return VectorP(PI * 0.5 + 16 * PI,1);
            
            //Breakpoiiiiiiint tho stop checking for averageeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
            //return photoResistors[index].position;

            //Calculate neighbour average
            float avrgAngle = 0;
            float sumValues = 0;
            for(int i = index - 1; i < index + 2; i++){
                //Handles exeptions when i is outide of array. Currently it adds the closest index inside the array to the total average angle
                if(i < 0){
                    float val = photoResistors[0].getLight();
                    sumValues += val;
                    VectorP polVect = photoResistors[0].position;
                    avrgAngle += polVect.angle * val;
                }
                else if(i > numPhotoResistors - 1){
                    float val = photoResistors[numPhotoResistors - 1].getLight();
                    sumValues += val;
                    VectorP polVect = photoResistors[numPhotoResistors - 1].position;
                    avrgAngle += polVect.angle * val;                    
                }
                else{
                    float val = photoResistors[i].getLight();
                    sumValues += val;
                    VectorP polVect = photoResistors[i].position;
                    avrgAngle += polVect.angle * val;                
                }
            }
            avrgAngle /= sumValues; //Definition for center of mass equation, exept this time it's a center of value equation
            return(VectorP(avrgAngle, 1));
        }
};
#endif