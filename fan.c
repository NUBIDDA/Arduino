#include <dht11.h>

#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
int T = 0;
int H = 0;
int R = 3;

void setup() {
  Serial.begin(9600);
  pinMode(R, OUTPUT);
}

void dht11() {
  T = dht.readTemperature();
  H = dht.readHumidity();
  Serial.print("Temperature : ");
  Serial.print(T);
  Serial.println("C");
  Serial.print("Humidity : ");
  Serial.print(H);
  Serial.println("%");  
}

void loop() {
  dht11();      // dht11 함수 실행 

  if (T >= 25) {
    digitalWrite(R, HIGH);
  } else {
    digitalWrite(R, LOW);
  }
  delay(1000);
}