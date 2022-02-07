#include<Servo.h>

//Pins
#define SERVOPINL 10
#define SERVOPINR 9


Servo servoLeft;
Servo servoRight;

void setup(){
    servoLeft.attach(SERVOPINL);
    servoRight.attach(SERVOPINR);
    servoLeft.write(90);
    servoRight.write(90);

    Serial.begin(9600);
}


void loop(){

    driveServos(-0.15, 0.15);
    delay(1000);
    driveServos(0.15, -0.15);
    delay(1000);
}

void driveServos(float leftPercentage, float rightPercentage){
    int left = floatMap(leftPercentage, -1, 1, 180, 0);
    int right = floatMap(rightPercentage, -1, 1, 0, 180);

    servoLeft.write(left);
    servoRight.write(right);
}

float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}