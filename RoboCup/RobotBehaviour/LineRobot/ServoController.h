#ifndef SERVOCONTROLLER_H
#define SERVOCONTROLLER_H

#include<Servo.h>
#include"MathFuncs.h"
#include"Vector.h"

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
#endif