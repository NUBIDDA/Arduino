#include "DHT.h" 

#include <Wire.h>
#define sv1 8
#include <SoftwareSerial.h>
#define DHTPIN 2       
#define DHTTYPE DHT11   
int relay = 10;         

DHT dht(DHTPIN, DHTTYPE);
 
void setup() {
  Wire.begin(sv1);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600); 
  Serial.println("Relay on");
  dht.begin();
  pinMode (relay, OUTPUT); 
}
 
void loop() {

  delay(1000);
}

void receiveEvent(int howMany){
   int temp = Wire.read();  // 온도
   int humi = Wire.read(); // 습도
   char mode = Wire.read();
   float di = discomfortIndex((float)temp, (float)humi);    // 불쾌지수값 
  Serial.print("온도는 ");
  Serial.print(temp);
  Serial.print("습도는 ");
  Serial.print(humi);
  Serial.print("di값은 ");
  Serial.print(di);
  Serial.print("\n");

  if(mode =='a') // 자동화
  {
    if(di>=80)
    {
      digitalWrite (relay, HIGH);
    } else {
      digitalWrite (relay, LOW);
    }
  } // if문

  else if (mode=='o')
  {
    Serial.print("회전");
    digitalWrite(relay, HIGH);
  }

  else if (mode =='c')
  {
    Serial.print("닫기");
    digitalWrite(relay,LOW);
  }
}
// Event 끝

// 불쾌지수 값 공식
float discomfortIndex(float temp, float humi) {
    return ( 1.8f * temp ) - ( 0.55 * (1 - humi / 100.0f ) * ( 1.8f * temp - 26 )) + 32;
}