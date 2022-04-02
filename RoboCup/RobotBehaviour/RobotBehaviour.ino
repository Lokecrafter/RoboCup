#include<Servo.h>
#include"RobotPeripherals.h"
#include"Vector.h"
#include"global.h"

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
float followLineThreshold = 0.9;  
#define FLOORDIST 12
#define BALLDIST 3

#define NUM_PHOTO_RESISTORS 10
int photoResistors[] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9};
//Robot 2 with protected lights
//int blackAdd = 25;
//int whiteValues[] = {160, 167, 159, 165, 151, 155, 194, 161, 170, 198};
//int blackValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//int whiteValues[] = {251, 229, 206, 198, 190, 191, 235, 207, 225, 277};
//int blackValues[] = {293, 261, 228, 214, 205, 207, 268, 264, 320, 403};
//int whiteValues[] = {251, 229, 206, 198, 190, 191, 235, 207, 225, 277};
//int blackValues[] = {293, 261, 228, 214, 205, 207, 268, 237, 250, 300};
//int whiteValues[] = {251, 229, 206, 198, 190, 191, 235, 207, 225, 277};
//int blackValues[] = {270, 261, 235, 220, 210, 207, 258, 237, 250, 300};

int whiteValues[] = {251, 229, 206, 198, 190, 191, 235, 207, 225, 277};
int blackValues[] = {290, 261, 235, 220, 210, 207, 258, 247, 260, 310};

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
UltraSonicSensor distSens(TRIGPIN, ECHOPIN);




void setup(){
    Serial.begin(9600);
	Serial.println("Started!");

	for(int i = 0; i < NUM_PHOTO_RESISTORS; i++){
		//blackValues[i] = whiteValues[i] + blackAdd;
		photoAngles[i].angle *= PI/180;
		photoPositions[i] = (Vector2)photoAngles[i];
	}

	lnFind.begin(NUM_PHOTO_RESISTORS, photoResistors, photoPositions, whiteValues, blackValues);
	mtrCtrl.begin(SERVOPINL, SERVOPINR);
	pickup.begin(SERVOTILT, SERVOCLAW);

    pinMode(ENABLEPIN, INPUT_PULLUP);
    pinMode(SWITCHOBSTACLE, INPUT_PULLUP);
    pinMode(SWITCHUNLOAD, INPUT_PULLUP);

	pickup.close();
}


void left(){
	float a = (OFFSETANGLE * PI / 180);
	VectorP direction = VectorP(0, 1);
	Vector2 vect = Vector2(0, 0);

	direction.angle = PI - a;

	vect = direction * -1;
	mtrCtrl.drive(vect, 0.15);
	delay(700);

    mtrCtrl.drive(vect, 0); //Stops robot
	delay(100);
}
void right(){
	float a = (OFFSETANGLE * PI / 180);
	VectorP direction = VectorP(0, 1);
	Vector2 vect = Vector2(0, 0);

	direction.angle = 0 - a;

	vect = direction * -1;
	mtrCtrl.drive(vect, 0.15);
	delay(1000);

    mtrCtrl.drive(vect, 0); //Stops robot
	delay(100);
}
void forward(int time){
	float a = ((OFFSETANGLE + 5) * PI / 180);
	VectorP direction = VectorP(0, 1);
	Vector2 vect = Vector2(0, 0);

	direction.angle = PI * 0.5 - a;

	vect = direction * -1;
	mtrCtrl.drive(vect, 0.15);
	delay(time);

    mtrCtrl.drive(vect, 0); //Stops robot
	delay(100);
}

void goAround(){
	Vector2 vect = Vector2(0, 1);
    mtrCtrl.drive(vect, 0.15); //Stops robot
	delay(400);

	left();
	forward(1400);
	right();
	forward(3000);
	right();
	forward(1500);
	left();

	vect = Vector2(0,0);
    mtrCtrl.drive(vect, 0); //Stops robot

	delay(500);
}

bool isWhite = false;
unsigned long whiteStartTime = 0;
unsigned long maxTimeAllowed = 3000;

void loop(){
	Vector2 vect(0,0);

    if(digitalRead(ENABLEPIN) == HIGH) {
		mtrCtrl.drive(vect,0);
        return;
    }

	VectorP lineDir = lnFind.getLine();
	if(lineDir.angle >= 8 * PI){
		if(isWhite){
			if(millis() >= whiteStartTime + maxTimeAllowed){
				resqueZone();
			}
		}
		else{
			whiteStartTime = millis();
		}
		isWhite = true;
	}
	else{
		isWhite = false;
	}

    vect = (Vector2)lineDir;
    vect.x *= -1;
    Serial.print(vect.x);
    Serial.print(", ");
    Serial.println(vect.y);


    if(digitalRead(SWITCHOBSTACLE) == HIGH) {
		//pickup.PickUp();
		goAround();
    }

	vect.y *= -1;
    mtrCtrl.drive(vect, 0.15);

}

//Enable resqueZone mode and never return. Will continue forever
void resqueZone(){
	while (true)
	{
		resqueUpdate();
	}
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