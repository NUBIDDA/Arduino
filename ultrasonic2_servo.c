#include <Servo.h>
const int servoPin = 10;  // 서보모터

Servo myServo;

int trig1 = 7;  // 초음파센서1
int echo1 = 5;

int trig2 = 4;  // 초음파센서2
int echo2 = 2;

void setup() {
  myServo.attach(servoPin);
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
}

void loop() {
  digitalWrite(trig1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1, LOW);    
  
  int duration1 = pulseIn(echo1, HIGH);
  int dis1 = duration1 / 29 / 2;
  if (dis1 < 9) {                  // 특정거리 이내에 물체가 있으면
    myServo.write(90);             // 서보모터 각도 동작
    delay(1000); 
  }
  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);   
  
  int duration2 = pulseIn(echo2, HIGH);
  int dis2 = duration2 / 29 / 2;
  if (dis2 < 9) {
    myServo.write(180);             // 이전 서보모터의 동작 각도와 반대값
    delay(1000);
  }
}