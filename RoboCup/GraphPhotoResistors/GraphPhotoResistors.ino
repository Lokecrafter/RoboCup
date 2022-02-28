const byte numPhotoResistors = 10;
const int photoResistors[] = {A8, A9, A0, A1, A2, A3, A4, A5, A7, A6};
int baseValues[] = {287,261,225,390,271,475,470,252,289,175};
int finValues[]  = {515,420,300,571,415,698,612,342,480,319};

void setup(){
    Serial.begin(9600);
}

void loop (){ 
  for(byte i = 0; i < numPhotoResistors; i++){
    int val = analogRead(photoResistors[i]);
    float mappedVal = floatMap(val, baseValues[i], finValues[i], 0, 10);
    /*
    Serial.print("[");
    Serial.print(i);
    Serial.print("]");
    Serial.print("(");
    Serial.print(val - baseValues[i]);
    Serial.print(")");
    Serial.print("(");
    Serial.print(finValues[i] - baseValues[i]);
    Serial.print(")");*/
    Serial.print(mappedVal);
    Serial.print("   ");
  }
  Serial.println("");
  delay(50);
}

float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int clamp(int x, int minV, int maxV){
  x = min(x, maxV);
  x = max(x, minV);
  return x;
}