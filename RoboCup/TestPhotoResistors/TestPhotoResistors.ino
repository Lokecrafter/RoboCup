


const int photoResistors[] = {A0, A1, A2, A3, A4, A5};
const byte numPhotoResistors = 6;

void setup(){
    for(byte i = 0; i < numPhotoResistors; i++){
        pinMode(photoResistors[i], INPUT);
    }
    Serial.begin(9600);
    Serial.println("Started!");
}

void loop (){
  
    for(byte i = 0; i < numPhotoResistors; i++){
        int val = analogRead(photoResistors[i]);
        Serial.print(val);
        Serial.print("   ");
    }
    Serial.println("");
    delay(50);
}
