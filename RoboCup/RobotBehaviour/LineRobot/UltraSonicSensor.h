#ifndef ULTRASONICSENSOR_H
#define ULTRASONICSENSOR_H
class UltraSonicSensor{
    private:
        int trigPin;
        int echoPin;
    public:
        void begin(int newTrigPin, int newEchoPin){
            trigPin = newTrigPin;
            echoPin = newEchoPin;

            pinMode(trigPin, OUTPUT);
            pinMode(echoPin, INPUT);
        }

        float getDistance(){
            digitalWrite(trigPin, LOW);
            delayMicroseconds(2);

            digitalWrite(trigPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(trigPin, LOW);

            float duration = pulseIn(echoPin, HIGH, 5000);

            if(duration == 0){
                //Absurdley large number to make next calculation very large. Method getDistance() will then return a large number to indicate that no echo was heard.
                duration = 100000000000; 
            }
            float distance = duration * 0.034 / 2;
            Serial.println(distance);
            return distance;
        }
};
#endif