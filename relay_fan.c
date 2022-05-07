#include "DHT.h" 

#define DHTPIN 2       
#define DHTTYPE DHT11   
int relay = 10;         

DHT dht(DHTPIN, DHTTYPE);
 
void setup() {
  Serial.begin(9600); 
  Serial.println("Relay on");
  dht.begin();
  pinMode (relay, OUTPUT); 
}
 
void loop() {
  float humi = dht.readHumidity();          // 습도값  
  float temp = dht.readTemperature();       // 온도값 
  float di = discomfortIndex(temp,humi);    // 불쾌지수값 
 
  if (isnan(temp) || isnan(humi)) {
    Serial.println("Failed to read from DHT");
  } else {
    // 습도, 온도, 불쾌지수값 시리얼 모니터 출력
    Serial.print("Humi: "); 
    Serial.print(humi);
    Serial.print(" %\t");
    Serial.print("Temp: "); 
    Serial.print(temp);
    Serial.print(" *C");
    Serial.print(" \t");
    Serial.print("DI: ");
    Serial.println(di);
 
    if(di>=80)
    {
      digitalWrite (relay, HIGH);
    } else {
      digitalWrite (relay, LOW);
    }
  }
  delay(2000);
}
 
// 불쾌지수 값 공식
float discomfortIndex(float temp, float humi) {
    return ( 1.8f * temp ) - ( 0.55 * (1 - humi / 100.0f ) * ( 1.8f * temp - 26 )) + 32;
}
 

 