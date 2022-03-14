int measurePin = 0;          // measurePin을 0으로 설정합니다.
int ledPower = 2;             // ledPower를 2로 설정합니다.
 
int samplingTime = 280;    // samplingTime을 280으로 설정합니다.
int deltaTime = 40;           // deltaTime을 40으로 설정합니다.
int sleepTime = 9680;       // sleepTime을 9690으로 설정합니다.
 
float voMeasured = 0;      // voMeasured를 0으로 설정합니다.
float calcVoltage = 0;       // calcVoltage를 0으로 설정합니다.
float dustDensity = 0;       // dustDensity를 0으로 설정합니다.


int green =12;
int yellow=10;
int red=8;


int ReadUVintensityPin = A1;
 
 
void setup(){
  Serial.begin(9600);                   // 시리얼 통신을 사용하기 위해 보드레이트를 9600으로 설정
  pinMode(ReadUVintensityPin, INPUT);//자외선
  
  
  
  
  pinMode(ledPower,OUTPUT); // ledPower를 출력 단자로 설정합니다.
  pinMode(green,OUTPUT);
  pinMode(yellow,OUTPUT);
  pinMode(red ,OUTPUT);
}
 
void loop(){

  int uvLevel = averageAnalogRead(ReadUVintensityPin);
  float outputVoltage = 5.0 * uvLevel/1024; //전압을 아날로그 신호 범위로 변환
  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.9, 0.0, 15.0); //UV 강도로 변환
  digitalWrite(ledPower,LOW);                               // ledPower를 LOW로 설정합니다.
  //---------------------------------------------------------------------------------(자외선)  

  
  delayMicroseconds(samplingTime);                    // samplingTime(280) 마이크로초만큼 지연합니다.

  voMeasured = analogRead(measurePin);             //  measurePin의 아날로그 값을 받아 voMeasured에 저장합니다.
 
  delayMicroseconds(deltaTime);                          // deltaTime(40) 마이크로초만큼 지연합니다.
  digitalWrite(ledPower,HIGH);                              // ledPower를 HIGH로 설정합니다.
  delayMicroseconds(sleepTime);                        // sleepTime(9680) 마이크로초만큼 지연합니다.
 
  calcVoltage = voMeasured * (5.0 / 1024.0);         // voMeasured의 값을 5.0/1024.0을 곱하여 calcVoltage에 저장합니다.
 
  dustDensity = (0.17 * calcVoltage - 0.1) * 1000;   // calcVoltage 값에 0.17을 곱하고 -0.1을 더합니다. (mg/m3)
                                                   // 값을 ug/m3 단위로 표현하기 위해 1000을 곱하여 dustDensity에 저장하여줍니다.
  //----------출력값                          
  digitalWrite(red,LOW);
  digitalWrite(yellow,LOW);
  digitalWrite(green,LOW);
  Serial.print("먼지:");
  Serial.print(dustDensity);                              // dustDensity을 시리얼 통신으로 출력합니다.
  Serial.print(" ug/m3 입니다.");           // " ug/m3"를 시리얼 통신으로 출력하고 줄을 바꿉니다.
  Serial.print(" 자외선: ");
  Serial.print(uvIntensity);
  Serial.println(" mW/cm^2 입니다.");
  //------------

//  Serial.print("UVAnalogOutput: ");
//  Serial.print(uvLevel);
// 
//  Serial.print(" OutputVoltage: ");
//  Serial.print(outputVoltage);

 

  if(dustDensity >0 && uvIntensity >0){

    if(dustDensity <30 && uvIntensity <0.2){ // 가림막 open
      re();
    }

    if(dustDensity <80 || uvIntensity <0.5){ // 가림막 45도
      gre();
    }

    if(dustDensity >80 || uvIntensity >0.5){ // 가림막 다 닫기
      yel();
    }
 
  }


  delay(5000);                                                 // 5초동안 지연합니다.
}


void re(){
        digitalWrite(red,HIGH);
        digitalWrite(green,LOW);
        digitalWrite(yellow,LOW);
}

void gre(){
        digitalWrite(red,LOW);
        digitalWrite(green,HIGH);
        digitalWrite(yellow,LOW);
}
void yel(){
        digitalWrite(red,LOW);
        digitalWrite(green,LOW);
        digitalWrite(yellow,HIGH);
}


int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 
 
  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;
 
  return(runningValue);  
 
}
 
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}