#include<Servo.h>
#include"LineRobot.h"

//Pins
#define SERVOPINL 10
#define SERVOPINR 9
#define ENABLEPIN 23
#define TRIGPIN 47
#define ECHOPIN 46

#define SERVOTILT 49
#define SERVOCLAW 51
#define SWITCHUNLOAD 25
#define SWITCHOBSTACLE 27

//Threshold for check if robot shpuld follow line oe continue forward
float followLineThreshold = 0.1;  
#define FLOORDIST 12
#define BALLDIST 3

#define NUM_PHOTO_RESISTORS 10
int photoResistors[] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9};

int whiteValues[] = {290, 260, 229, 215, 209, 205, 250, 219, 230, 280};
int blackValues[] = {395, 343, 288, 250, 245, 231, 286, 262, 278, 347};

float avoidDistance = 5;

VectorP photoAngles[NUM_PHOTO_RESISTORS] = {
	VectorP(90 + 120, 1),
	VectorP(90 + 80, 1),

	VectorP(90 + 50, 1),
	VectorP(90 + 15, 1),
	VectorP(90 + 5, 1),
	VectorP(90 - 5, 1),
	VectorP(90 - 15, 1),
	VectorP(90 - 50, 1),
	
	VectorP(90 - 80, 1),
	VectorP(90 - 120, 1)
};
Vector2 photoPositions[NUM_PHOTO_RESISTORS];





//Functional objects. These enable robot driving
LineFinder lnFind;
ServoController mtrCtrl;
BallPickup pickup;
UltraSonicSensor distSens;




void setup(){
    Serial.begin(9600);
	Serial.println("Started!");

	lnFind.begin(NUM_PHOTO_RESISTORS, photoResistors, photoPositions, whiteValues, blackValues);
	mtrCtrl.begin(SERVOPINL, SERVOPINR);
	pickup.begin(SERVOTILT, SERVOCLAW);
	distSens.begin(TRIGPIN, ECHOPIN);

    pinMode(ENABLEPIN, INPUT_PULLUP);
    pinMode(SWITCHOBSTACLE, INPUT_PULLUP);
    pinMode(SWITCHUNLOAD, INPUT_PULLUP);

	pickup.close();
}

void loop(){
	Vector2 vect(0,0);

    if(digitalRead(ENABLEPIN) == HIGH) {
		mtrCtrl.drive(vect,0);
        return;
    }

	VectorP lineDir = lnFind.getLineAng();
	
    vect = (Vector2)lineDir;
    vect = vect * -1;
    mtrCtrl.drive(vect, 0.15);
}


void resqueUpdate(){
	Vector2 vect(0,0);
	if(digitalRead(ENABLEPIN) == HIGH) {
		mtrCtrl.drive(vect,0);
		return;
	}
	mtrCtrl.drive(vect,0);



	if(distSens.getDistance() <= FLOORDIST - BALLDIST){
		pickup.close();
		delay(1000);
	}
	else{
		pickup.open();
	}

	vect = Vector2(0,1);
	mtrCtrl.drive(vect,0.15);

}