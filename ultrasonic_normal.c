#define TRIG 2    // TRIG 핀 설정 (초음파 보내는 핀)
#define ECHO 4    // ECHO 핀 설정 (초음파 받는 핀)

void setup() { 
  Serial.begin(9600);          
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(3, OUTPUT); 
}

void loop() {

  long duration, distance;
  
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  
  duration = pulseIn (ECHO, HIGH); 
  distance = duration * 17 / 1000;
  
  Serial.println(duration);            // 초음파가 반사되어 돌아오는 시간
  Serial.print("\nDIstance : ");
  Serial.print(distance);              // 측정된 물체로부터 거리값(cm값)
  Serial.println(" Cm");

  if (distance > 0) {
    if(distance < 20) {
      analogWrite(3, 130);
      delay(1000);
    }
    else if(distance < 40){
      analogWrite(3, 80);
      delay(1000);
    }
    else if (distance < 60){
      analogWrite(3, 50);
      delay(1000);
    }
    else {
      analogWrite(3, 0);
      delay(1000);
    };
  };
  delay(1000);                         
}