#include<Arduino.h>
#include"Vector.h"
class LineFinder {
    private:
        int* photoResistors;
        int numPhotoResistors;

        int* whiteVals;
        int* blackVals;
        VectorP* photoAngles;

        float threshold = 0;

        float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
            return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
        }
    public:
        //Setters
        void SetNumPhotoResistors(int newNum){numPhotoResistors = newNum;}
        void SetThreshold(float newThreshold){threshold = newThreshold;}
        void SetPhotoResistors(int* newPins){
            //delete [] photoResistors;
            photoResistors = newPins;
        }
        void SetWhiteVals(int* newVals){
            delete [] whiteVals;
            whiteVals = newVals;
        }
        void SetBlackVals(int* newVals){
            delete [] blackVals;
            blackVals = newVals;
        }
        void SetAngles(VectorP* newAngles){ //Give angles in degrees
            delete [] photoAngles;
            photoAngles = newAngles;
            for(int i = 0; i < numPhotoResistors; i++){
                photoAngles[i].angle *= PI / 180;
            }
        }

        //Constructor & Destructor
        //Instantiate linefinder with pins to use and the amount of pins
        LineFinder(int* photoResistorPins, int numPins){
            SetPhotoResistors(photoResistorPins);
            SetNumPhotoResistors(numPins);
        }
        ~LineFinder(){
            //Free memory and give back to OS
            delete [] photoResistors;
            delete [] whiteVals;
            delete [] blackVals;
            delete [] photoAngles;

            photoResistors = nullptr;
            whiteVals = nullptr;
            blackVals = nullptr;
            photoAngles = nullptr;
        }

        //Methods
        VectorP calcLineMiddle(){
            //Find max value
            int index = 0;
            float maxVal = 0;
            for(byte i = 0; i < numPhotoResistors; i++){
                float val = (float)analogRead(photoResistors[i]);
                val = floatMap(val, whiteVals[i], blackVals[i], 0, 1);
                if(maxVal < val){
                    maxVal = val;
                    index = i;
                }
            }
            //Return the vector pointing straight forward if none of photo resistors are black enough
            if(maxVal <= threshold) return VectorP(PI * 0.5,1);

            //Calculate neighbour average
            float avrgAngle = 0;
            if(index <= 0)
                avrgAngle = (photoAngles[0].angle + photoAngles[0].angle + photoAngles[1].angle) / 3;
            else if(index >= numPhotoResistors - 1)
                avrgAngle = (photoAngles[numPhotoResistors - 2].angle + photoAngles[numPhotoResistors - 1].angle + photoAngles[numPhotoResistors - 1].angle) / 3;
            else
                avrgAngle = (photoAngles[index - 1].angle + photoAngles[index].angle + photoAngles[index + 1].angle) / 3;
            return(VectorP(avrgAngle, 1));
        }
};







#include<Servo.h>
class MotorController{
    private:
        Servo servoLeft;
        Servo servoRight;

        float clamp(float val, float minVal, float maxVal){
            val = max(val, minVal);
            val = min(val, maxVal);
            return(val);
        }
        float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
            return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
        }
    public:
        MotorController(int leftServoPin, int rightServoPin){
            servoLeft.attach(leftServoPin);
            servoRight.attach(rightServoPin);
            servoLeft.write(90);
            servoRight.write(90);
        }

        void driveServos(Vector2 &vect, float speedMultiplier){
            vect.x = clamp(vect.x, -1, 1);
            vect.y = clamp(vect.y, -1, 1);

            vect = vect * (speedMultiplier);

            //Only when using a controller
            //if(vect.y < 0)  vect.y = vect.y * -1; //Handles steering so it feels natural when robot is reversing. With this if-statement it simulates how a car would steer when reversing

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




class UltraSonicSensor{
    private:
        int trigPin;
        int echoPin;
    public:
        UltraSonicSensor(int newTrigPin, int newEchoPin){
            trigPin = newTrigPin;
            echoPin = newEchoPin;
        }

        float getDistance(){
            digitalWrite(trigPin, LOW);
            delayMicroseconds(2);

            digitalWrite(trigPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(trigPin, LOW);

            float duration = pulseIn(echoPin, HIGH, 5000);

            if(duration == 0){
            duration = 100000000000; //Absurdley large number to make next calculation very large 
            }
            float distance = duration * 0.034 / 2;
            Serial.println(distance);
            return distance;
        }
};