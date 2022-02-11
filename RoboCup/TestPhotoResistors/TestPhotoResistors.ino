const int photoResistors[] = {A0, A1, A2, A3, A4, A5};
int baseValues[] = {0,0,0,0,0,0};
int finValues[] = {0,0,0,0,0,0};
const byte numPhotoResistors = 6;

void setup(){
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

void loop (){
  /*
    for(byte i = 0; i < numPhotoResistors; i++){
        int val = analogRead(photoResistors[i]);
        val = floatMap(val, baseValues[i], finValues[i], 0, 10);
        Serial.print(val);
        Serial.print("   ");
    }
    Serial.println("");
    delay(50);
    */
 //   return;
    Serial.println(calcLineMiddle(), 4);
    delay(50);
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

float indexTo01(float index, float count){
  return((2/(count - 1))*index - 1);
}

float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
