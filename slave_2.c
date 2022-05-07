
#define sv1 9
#include <Servo.h>

  
#include <Wire.h>   //i2c
Servo servo;
Servo servo2;
byte i = 0;

void setup() {
  Wire.begin(sv1);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
   servo.attach(7);
  servo2.attach(8);
}

void loop() {
  delay(100);
  
}

void receiveEvent(int howMany) {
  
    int uv = Wire.read();
    int dust = Wire.read();
    char mode = Wire.read();

    float new_uv = uv/100;
    float new_dust = dust/100;
    
    if (mode=='a'){
      Serial.print("자동화 모드 입니다.");
      Serial.print("\n");
      
      Serial.print(uv);
      Serial.print(",");
      Serial.print(dust);
      Serial.print("\n");

      if(dust >50 && uv >0.3){
         Open();
      }
      else{
        Close();
      }
    }
    if(mode=='o'){
      Serial.print("열어주세요");
      Serial.print("\n");
      Open();
    }
    if (mode=='c'){
      Serial.print("닫아주세요");
      Serial.print("\n");
      Close();
    }
}


void Open(){
  servo.write(180);
  servo2.write(0);
}

void Close(){
  servo.write(0);
  servo2.write(180);
}
  
