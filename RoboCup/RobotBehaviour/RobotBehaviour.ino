//#include<Servo.h>
//#include"Vector.h"
#include"RobotPeripherals.h"

//Pins
#define SERVOPINL 10
#define SERVOPINR 9
#define ENABLEPIN 23
#define TRIGPIN 30
#define ECHOPIN 32

float followLineThreshold = 0.7;  //Threshold for check if robot shpuld follow line oe continue forward

int numPhotoResistors = 10;
int photoResistors[] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9};
/*
int whiteValues[] = {284,195,213,255,200,265,334,314,335,250};
int blackValues[] = {694,425,408,490,321,498,576,322,399,390};
*/
/*
//Robot 2
int whiteValues[] = {150,142,136,145,131,133,163,131,135,177};
int blackValues[] = {170,160,154,160,150,150,183,150,155,194};
*/
//Robot 2 with protected lights
int blackAdd = 25;
int whiteValues[] = {160, 167, 159, 165, 151, 155, 194, 161, 170, 198};
int blackValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

float avoidDistance = 5;

VectorP photoAngles[] = {
	VectorP(90 + 120, 1),
	VectorP(90 + 90, 1),

	VectorP(90 + 15, 1),
	VectorP(90 + 10, 1),
	VectorP(90 + 5, 1),
	VectorP(90 - 5, 1),
	VectorP(90 - 10, 1),
	VectorP(90 - 15, 1),
	
	VectorP(90 - 90, 1),
	VectorP(90 - 120, 1)
};

LineFinder lnFind;
MotorController mtrCtrl;
//UltraSonicSensor distSens(TRIGPIN, ECHOPIN);

void setup(){
    Serial.begin(9600);
	Serial.println("Started!");

	lnFind.begin(photoResistors, numPhotoResistors);
	mtrCtrl.begin(SERVOPINL, SERVOPINR);

	for(int i = 0; i < numPhotoResistors; i++){
		blackValues[i] = whiteValues[i] + blackAdd;
	}

	lnFind.SetThreshold(followLineThreshold);
	lnFind.SetBlackVals(blackValues);
	lnFind.SetWhiteVals(whiteValues);
	lnFind.SetAngles(photoAngles);



    pinMode(ENABLEPIN, INPUT_PULLUP);
}


void loop(){
    if(digitalRead(ENABLEPIN) == HIGH) {
		Vector2 vect(0,0);

		mtrCtrl.driveServos(vect,0);

        return;
    }
	/*
  	if(distSens.getDistance() <= avoidDistance){
  		Serial.println("Abort! Abort! AAAAABOOOOORTTTT!");
	}*/
    Vector2 vect = lnFind.calcLineMiddle().ToVector2();
    vect.x = vect.x * -1;

    Serial.print(vect.x);
    Serial.print(", ");
    Serial.println(vect.y);

    mtrCtrl.driveServos(vect, 0.15);

}
