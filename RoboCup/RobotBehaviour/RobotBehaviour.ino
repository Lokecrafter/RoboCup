#include<Servo.h>
#include"Vector.h"
#include"RobotPeripherals.h"

//Pins
#define SERVOPINL 10
#define SERVOPINR 9
#define ENABLEPIN 23
#define TRIGPIN 30
#define ECHOPIN 32

float followLineThreshold = 0.7;  //Threshold for check if robot shpuld follow line oe continue forward

int numPhotoResistors = 10;
int photoResistors[] = {A8, A9, A0, A1, A2, A3, A4, A5, A7, A6};
//Försök 1 Teknikum
//int baseValues[] = {287,261,225,390,271,475,470,252,289,175};
//int finValues[]  = {515,420,300,571,415,698,612,342,480,319};
//Försök 2 Teknikum
//const int whiteValues[] = {288,265, 227,211,155,216,260,193,288,204};
//const int blackValues[] = {539,449,332,369,273,430,497,319,407,369};
//Försök 1 RoboCup
//const int whiteValues[] = {530,410,290,215,167,230,287,272,455,460};
//const int blackValues[] = {790,650,470,430,335,470,595,435,520,600};
//Försök lampor extra
//const int whiteValues[] = {343,226,263,246,198,229,340,284,312,220};
//const int blackValues[] = {655,270,380,498,373,647,615,492,495,520};
//Försök 2 lampor extra
int whiteValues[] = {284,195,213,255,200,265,334,314,335,250};
int blackValues[] = {694,425,408,490,321,498,576,322,399,390};
//Lek
//const int whiteValues[] = {284,195,213,255,200,265,334,314,335,250};
//const int blackValues[] = {674,405,388,470,301,478,556,302,379,370};

float avoidDistance = 5;
/*
Vector2 photoPositions[] = { 
    Vector2(-4.5,-1), 
    Vector2(-3.5,0),

    Vector2(-2.5,1), 
    Vector2(-1.5,1), 
    Vector2(-0.5,2), 
    Vector2(0.5,2), 
    Vector2(1.5,1), 
    Vector2(2.5,1),  

    Vector2(3.5,0),
    Vector2(4.5,-1)
};*/
Vector2 photoPositions[] = { 
    Vector2(-4.5,-1), 
    Vector2(-3.5,0),

    Vector2(-2.5,1), 
    Vector2(-1.5,1), 
    Vector2(-0.5,2), 
    Vector2(0.5,2), 
    Vector2(1.5,1), 
    Vector2(2.5,1),  

    Vector2(3.5,0),
    Vector2(4.5,-1)
};

LineFinder lnFind(photoResistors, numPhotoResistors);
MotorController mtrCtrl(SERVOPINL, SERVOPINR);

void setup(){
	lnFind.SetThreshold(followLineThreshold);
	lnFind.SetBlackVals(blackValues);
	lnFind.SetWhiteVals(whiteValues);


    pinMode(ENABLEPIN, INPUT_PULLUP);
    pinMode(TRIGPIN, OUTPUT);
    pinMode(ECHOPIN, INPUT);

    Serial.begin(9600);
}


void loop(){
    if(digitalRead(ENABLEPIN) == HIGH) {

      return;
    }

    Vector2 vect = lnFind.calcLineMiddle().ToVector2();
    vect.x = vect.x * -1;

    mtrCtrl.driveServos(vect, 0.15);

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
