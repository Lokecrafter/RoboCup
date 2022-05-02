#ifndef BALLPICKUP_H
#define BALLPICKUP_H
class BallPickup{
    private: 
        Servo tilt;
        Servo grip;

        int startTilt;
        int startGrip;
        int endTilt;
        int endGrip;
    public:
        void PositionManually();
        void begin(int newTiltPin, int newClawPin, int startTiltAngle = 125, int startGripAngle = 40, int endTiltAngle = 30, int endGripAngle = 180){
            tilt.attach(newTiltPin);
            grip.attach(newClawPin);

            SetStartAngles(startTiltAngle, startGripAngle);
            SetEndAngles(endTiltAngle, endGripAngle);

            PositionManually(0,0);
        }
        void SetStartAngles(int tiltAngle, int gripAngle){
            startTilt = tiltAngle;
            startGrip = gripAngle;
        }
        void SetEndAngles(int tiltAngle, int gripAngle){
            endTilt = tiltAngle;
            endGrip = gripAngle;
        }


        void PositionManually(float tiltPercentage, float gripPercentage){
            tilt.write(round(floatMap(
                tiltPercentage, 0, 1, startTilt, endTilt
            )));
            grip.write(round(floatMap(
                gripPercentage, 0, 1, startGrip, endGrip
            )));
        }
        void PickUp(){
            PositionManually(0, 1);
            delay(500);
            PositionManually(1, 1);
            delay(1000);
            PositionManually(1, 0.2);
            delay(500);
            PositionManually(0, 0);
        }
        void open(){
            PositionManually(0, 1);
            delay(500);
            PositionManually(1, 1);
            delay(1000);
        }
        void close(){
            PositionManually(1, 0.2);
            delay(500);
            PositionManually(0, 0);
            delay(1000);
        }
};
#endif