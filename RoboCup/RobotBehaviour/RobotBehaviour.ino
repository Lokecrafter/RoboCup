#include<Servo.h>
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
int photoResistors[] = {A8, A9, A0, A1, A2, A3, A4, A5, A7, A6};

int whiteValues[] = {284,195,213,255,200,265,334,314,335,250};
int blackValues[] = {694,425,408,490,321,498,576,322,399,390};


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

LineFinder lnFind(photoResistors, numPhotoResistors);
MotorController mtrCtrl(SERVOPINL, SERVOPINR);
UltraSonicSensor distSens(TRIGPIN, ECHOPIN);

void setup(){
	lnFind.SetThreshold(followLineThreshold);
	lnFind.SetBlackVals(blackValues);
	lnFind.SetWhiteVals(whiteValues);
	lnFind.SetAngles(photoAngles);


    pinMode(ENABLEPIN, INPUT_PULLUP);
    pinMode(TRIGPIN, OUTPUT);
    pinMode(ECHOPIN, INPUT);

    Serial.begin(9600);
}


void loop(){
    if(digitalRead(ENABLEPIN) == HIGH) {

      return;
    }

	if(distSens.getDistance() <= avoidDistance){
		Serial.println("Abort! Abort! AAAAABOOOOORTTTT!");
	}

    Vector2 vect = lnFind.calcLineMiddle().ToVector2();
    vect.x = vect.x * -1;

    mtrCtrl.driveServos(vect, 0.15);

}