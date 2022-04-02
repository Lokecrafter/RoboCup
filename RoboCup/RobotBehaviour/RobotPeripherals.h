#include<Arduino.h>
#include"Vector.h"
#include"global.h"

//Prevent multiple definitions
#ifndef ROBOT_PERIPHERALS_H
#define ROBOT_PERIPHERALS_H


//Global useful methods
float clamp(float val, float minVal, float maxVal){
    val = max(val, minVal);
    val = min(val, maxVal);
    return(val);
}
float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

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
        //Constructor & Destructor
        //Instantiate linefinder with pins to use and the amount of pins
        void begin(int numPins, int* photoResistorPins, Vector2* photoPositions, int* whiteValues, int* blackValues){
            SetNumPhotoResistors(numPins);
            SetPhotoResistors(photoResistorPins, photoPositions, whiteValues, blackValues);
        }
        ~LineFinder(){
            //Free memory and give back to OS
            delete [] photoResistors;
            photoResistors = nullptr;
        }



        //Methods
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
            if(maxVal <= threshold) return VectorP(PI * 0.5 - (OFFSETANGLE * PI / 180) + 16 * PI,1);
            
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



#include<Servo.h>
class ServoController{
    private:
        Servo servoLeft;
        Servo servoRight;

    public:
        void begin(int leftServoPin, int rightServoPin){
            servoLeft.attach(leftServoPin);
            servoRight.attach(rightServoPin);
            servoLeft.write(90);
            servoRight.write(90);
        }


        //Vect is a joystick input where y-axis determines forward and backwardsspeed and x-axis determines turn rate   
        //Additonal notes:
            //When using an actual controller. Add this to your code:
            //if(vect.y < 0)  vect.y = vect.y * -1; 
            //It inverts steering when reversinng. It simulates how a car would steer when reversing
        void drive(Vector2 &vect, float speedMultiplier){
            vect.x = clamp(vect.x, -1, 1);
            vect.y = clamp(vect.y, -1, 1);

            vect = vect * speedMultiplier;

            float leftPercentage = vect.y + vect.x;
            float rightPercentage = vect.y - vect.x;
            leftPercentage = clamp(leftPercentage, -1, 1);
            rightPercentage = clamp(rightPercentage, -1, 1);

            int right = floatMap(leftPercentage, -1, 1, 180, 0);
            int left = floatMap(rightPercentage, -1, 1, 0, 180);

            servoLeft.write(left);
            servoRight.write(right);
        }
};


class CrossroadBias{
    private:

    public:
        CrossroadBias(){

        }
};


class UltraSonicSensor{
    private:
        int trigPin;
        int echoPin;
    public:
        UltraSonicSensor(int newTrigPin, int newEchoPin){
            trigPin = newTrigPin;
            echoPin = newEchoPin;

            pinMode(trigPin, OUTPUT);
            pinMode(echoPin, INPUT);
        }

        float getDistance(){
            digitalWrite(trigPin, LOW);
            delayMicroseconds(2);

            digitalWrite(trigPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(trigPin, LOW);

            float duration = pulseIn(echoPin, HIGH, 5000);

            if(duration == 0){
                //Absurdley large number to make next calculation very large. Method getDistance() will then return a large number to indicate that no echo was heard.
                duration = 100000000000; 
            }
            float distance = duration * 0.034 / 2;
            Serial.println(distance);
            return distance;
        }
};


class BallPickup{
    private: 
        Servo tilt;
        Servo grip;

        int startTilt;
        int startGrip;
        int endTilt;
        int endGrip;
    public:
        void PositionManually();
        void begin(int newTiltPin, int newClawPin, int startTiltAngle = 125, int startGripAngle = 40, int endTiltAngle = 30, int endGripAngle = 180){
            tilt.attach(newTiltPin);
            grip.attach(newClawPin);

            SetStartAngles(startTiltAngle, startGripAngle);
            SetEndAngles(endTiltAngle, endGripAngle);

            PositionManually(0,0);
        }
        void SetStartAngles(int tiltAngle, int gripAngle){
            startTilt = tiltAngle;
            startGrip = gripAngle;
        }
        void SetEndAngles(int tiltAngle, int gripAngle){
            endTilt = tiltAngle;
            endGrip = gripAngle;
        }


        void PositionManually(float tiltPercentage, float gripPercentage){
            tilt.write(round(floatMap(
                tiltPercentage, 0, 1, startTilt, endTilt
            )));
            grip.write(round(floatMap(
                gripPercentage, 0, 1, startGrip, endGrip
            )));
        }
        void PickUp(){
            PositionManually(0, 1);
            delay(500);
            PositionManually(1, 1);
            delay(1000);
            PositionManually(1, 0.2);
            delay(500);
            PositionManually(0, 0);
        }
        void open(){
            PositionManually(0, 1);
            delay(500);
            PositionManually(1, 1);
            delay(1000);
        }
        void close(){
            PositionManually(1, 0.2);
            delay(500);
            PositionManually(0, 0);
            delay(1000);
        }
};


#endif