// 주행 도로 기울기에 따라 모터 구동 완료

#include <SoftwareSerial.h>
#include <SparkFun_ADXL345.h>  

ADXL345 adxl = ADXL345(); 

int RX=8;
int TX=7;

const int RIGHT1 = 9; 
const int RIGHT2 = 10;
const int LEFT1 = 5; 
const int LEFT2 = 6;

void setup() 
{ 
  Serial.begin(9600); 
  pinMode(RIGHT1, OUTPUT); 
  pinMode(RIGHT2, OUTPUT);
  pinMode(LEFT1, OUTPUT); 
  pinMode(LEFT2, OUTPUT);
  
  adxl.powerOn();                     // Power on the ADXL345

  adxl.setRangeSetting(16);
  adxl.setSpiBit(0);                  
  adxl.setActivityXYZ(1, 0, 0);     
  adxl.setActivityThreshold(75);   
 
  adxl.setInactivityXYZ(1, 0, 0);   
  adxl.setInactivityThreshold(75);   
  adxl.setTimeInactivity(10);        

  adxl.setTapDetectionOnXYZ(0, 0, 1); 

  adxl.setTapThreshold(50);         
  adxl.setTapDuration(15);       
  adxl.setDoubleTapLatency(80);       
  adxl.setDoubleTapWindow(200);      
  
  adxl.setFreeFallThreshold(7);    
  adxl.setFreeFallDuration(30);     
                                    
  adxl.ActivityINT(1);
  adxl.FreeFallINT(1);
  adxl.doubleTapINT(1);
  adxl.singleTapINT(1);
} 

// 전진: RIGHT1 = LOW, RIGHT2 = HIGH / LEFT1 = LOW / LEFT2 = HIGH
// 양 쪽 바퀴 동시 전진 (0~255 속도 제어)
void forward() {
   // 추후 기울기 정도에 따라 바퀴 구동 속도 조절 기능 추가 (간단한 수학식 사용)
    analogWrite(RIGHT1, 0); 
    analogWrite(LEFT1, 0);
    analogWrite(RIGHT2, 50);
    analogWrite(LEFT2, 50);
}

void backward() {
    analogWrite(RIGHT1, 50);
    analogWrite(LEFT1, 50);
    analogWrite(RIGHT2, 0);
    analogWrite(LEFT2, 0);
}

void stop_run() {
   analogWrite(LEFT1, LOW); 
   analogWrite(LEFT2, LOW); 
   analogWrite(RIGHT1, LOW); 
   analogWrite(RIGHT2, LOW);
}
  void loop() {
  int x,y,z;   
  adxl.readAccel(&x, &y, &z);
  // 주행 상태 확인
  Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", "); Serial.println(z); 
  
   Serial.println("평지 주행 중입니다.");
   stop_run();
  if(y > 20 && z > 20) {  // 오르막길 - 주행을 돕기 위해 바퀴를 주행방향으로 구동
    Serial.println("오르막길 주행 중입니다.");
    forward();
   } else if(y < -20 && z > 20) {  // 내리막길 - 안정적인 주행을 돕기 위해 바퀴를 주행방향 반대로 구동
      Serial.println("내리막길 주행 중입니다.");
      backward();
    } else if(z < 15) {  // 낙상 감지
      Serial.println("낙상이 감지되었습니다. 괜찮으십니까?");
      stop_run();
    }
    delay(1000);  // delay() 삭제 후 구동
}