#include<Servo.h>
#include"Vector.h"

//Pins
#define SERVOPINL 10
#define SERVOPINR 9
#define ENABLEPIN 23
#define TRIGPIN 30
#define ECHOPIN 32


Servo servoLeft;
Servo servoRight;

const byte numPhotoResistors = 10;
const int photoResistors[] = {A8, A9, A0, A1, A2, A3, A4, A5, A7, A6};
int baseValues[] = {287,261,225,390,271,475,470,252,289,175};
int finValues[]  = {515,420,300,571,415,698,612,342,480,319};
float avoidDistance = 15;

Vector2 photoPositions[] = { 
    Vector2(-4.5,-1), 
    Vector2(-3.5,0),

    Vector2(-2.5,1), 
    Vector2(-1.5,1), 
    Vector2(-0.5,1), 
    Vector2(0.5,1), 
    Vector2(1.5,1), 
    Vector2(2.5,1),  

    Vector2(3.5,0),
    Vector2(4.5,-1)
};

void driveServos(Vector2* vect, float speedMultiplier);

void setup(){
    servoLeft.attach(SERVOPINL);
    servoRight.attach(SERVOPINR);
    servoLeft.write(90);
    servoRight.write(90);
    pinMode(ENABLEPIN, INPUT_PULLUP);
    pinMode(TRIGPIN, OUTPUT);
    pinMode(ECHOPIN, INPUT);

    Serial.begin(9600);
}


void loop(){
    //Serial.println(calcLineMiddle().x);

    Vector2 vect = calcLineMiddle();
    vect.x = vect.x * -1;
    /*
    Serial.print(vect.x);
    Serial.print("   ");
    Serial.println(vect.y);
    */
    if(getUltrasonicDistance(TRIGPIN, ECHOPIN) < avoidDistance) vect = vect * -1;

    if(digitalRead(ENABLEPIN) == LOW) driveServos(vect, 0.1);
    else{
      servoLeft.write(90);
      servoRight.write(90);
    }
    //delay(50);
}
/*
void driveServos(float leftPercentage, float rightPercentage, float speedMultiplier){
    leftPercentage *= speedMultiplier;
    rightPercentage *= speedMultiplier;

    int left = floatMap(rightPercentage, -1, 1, 0, 180);
    int right = floatMap(leftPercentage, -1, 1, 180, 0);

    servoLeft.write(left);
    servoRight.write(right);
}*/
void driveServos(Vector2 &vect, float speedMultiplier){
    vect.x = clamp(vect.x, -1, 1);
    vect.y = clamp(vect.y, -1, 1);

    vect = vect * speedMultiplier;

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
Vector2 calcLineMiddle(){
  Vector2 sum = Vector2(0,0);
  float valSum = 0;

  for(byte i = 0; i < numPhotoResistors; i++){
        float val = (float)analogRead(photoResistors[i]);
        val = floatMap(val, baseValues[i], finValues[i], 0, 1);
        valSum += val;
        sum = sum + (photoPositions[i] * val);
    }
  if(valSum == 0) return Vector2(0,0);
  return(sum / valSum);
}
float getUltrasonicDistance(int trigPin, int echoPin){
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

float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
float indexTo01(float index, float count){
  return((2/(count - 1))*index - 1);
}
float clamp(float val, float minVal, float maxVal){
    val = max(val, minVal);
    val = min(val, maxVal);

    return(val);
}
