class Vector2
{
    public:
        float x = 0;
        float y = 0;

        Vector2(float newX, float newY){
            x = newX;
            y = newY;
        }
        Vector2 operator + (Vector2 const &obj) {
            Vector2 vect(0,0);
            vect.x = x + obj.x;
            vect.y = y + obj.y;
            return vect;
        }
        Vector2 operator - (Vector2 const &obj) {
            Vector2 vect(0,0);
            vect.x = x - obj.x;
            vect.y = y - obj.y;
            return vect;
        }
        Vector2 operator * (float const &factor) {
            Vector2 vect(0,0);
            vect.x = x * factor;
            vect.y = y * factor;
            return vect;
        }
        Vector2 operator / (float const &factor) {
            Vector2 vect(0,0);
            vect.x = x / factor;
            vect.y = y / factor;
            return vect;
        }

        void normalize(){
            float length = sqrt(x*x + y*y);
            x /= length;
            y /= length;
        }
};



Servo servoLeft;
Servo servoRight;

const int photoResistors[] = {A0, A1, A2, A3, A4, A5};
int baseValues[] = {0,0,0,0,0,0};
int finValues[] = {0,0,0,0,0,0};
const byte numPhotoResistors = 6;

void driveServos(Vector2* vect, float speedMultiplier);

void setup(){
    servoLeft.attach(SERVOPINL);
    servoRight.attach(SERVOPINR);
    servoLeft.write(90);
    servoRight.write(90);

    //Calibrate white and black values
    for(byte i = 0; i < numPhotoResistors; i++){
        pinMode(photoResistors[i], INPUT);
    }
    delay(50);
    for(byte i = 0; i < numPhotoResistors; i++){
        baseValues[i] = analogRead(photoResistors[i]);
    }
    delay(5000);
    for(byte i = 0; i < numPhotoResistors; i++){
        finValues[i] = analogRead(photoResistors[i]);
    }
    Serial.begin(9600);
}


void loop(){
    Serial.println(calcLineMiddle(), 4);

    Vector2* vect = new Vector2(-calcLineMiddle(), 1);

    driveServos(vect, 0.1);
    delay(50);
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
void driveServos(Vector2 vect, float speedMultiplier){
    vect.x = clamp(vect.x, -1, 1);
    vect.y = clamp(vect.y, -1, 1);

    vect = vect * speedMultiplier;

    if(vect.y < 0)  vect.y = vect.y * -1; //Handles steering so it feels natural when robot is reversing. With this if-statement it simulates how a car would steer when reversing

    float leftPercentage = vect.y + vect.x;
    float rightPercentage = vect.y - vect.x;
    leftPercentage = clamp(leftPercentage, -1, 1);
    rightPercentage = clamp(rightPercentage, -1, 1);

    int right = floatMap(leftPercentage, -1, 1, 180, 0);
    int left = floatMap(rightPercentage, -1, 1, 0, 180);

    servoLeft.write(left);
    servoRight.write(right);
}
float calcLineMiddle(){
  float sum = 0;
  float valSum = 0;
  const int distBetween = 1;
  for(byte i = 0; i < numPhotoResistors; i++){
        int val = analogRead(photoResistors[i]);
        val = floatMap(val, baseValues[i], finValues[i], 0, 1);
        valSum += val;
        sum += val * indexTo01(i, 6) * distBetween;
    }
  if(valSum == 0) return 0;
  return(sum / valSum);
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
