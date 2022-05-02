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

        //vect - joystick input as a cartesian 2D-vector
        //speedMultiplier - speed multiplier between 0 and 1
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
        //ploVect - joystick input as a polar 2D-vector in terms of distance and degrees
        //speedMultiplier - speed multiplier between 0 and 1
        void drive(VectorP &polVect, float speedMultiplier){
            polVect.toDegrees();
            Vector2 vect = (Vector2)polVect;
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