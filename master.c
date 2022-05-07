
#include <SoftwareSerial.h>
#include <Wire.h>   //i2c

// i2c 용
#define sv1 8
#define sv2 9




int measurePin = 0;          // measurePin을 0으로 설정합니다.
int ledPower = 4;             // ledPower를 2로 설정합니다. 
 
int samplingTime = 280;    // samplingTime을 280으로 설정합니다.
int deltaTime = 40;           // deltaTime을 40으로 설정합니다.
int sleepTime = 9680;       // sleepTime을 9690으로 설정합니다.
 
float voMeasured = 0;      // voMeasured를 0으로 설정합니다.
float calcVoltage = 0;       // calcVoltage를 0으로 설정합니다.
float dustDensity = 0;       // dustDensity를 0으로 설정합니다.

void start();


// --- 자외선용
int ReadUVintensityPin = A1;
// --


SoftwareSerial BTSerial(2, 3); //Tx :2 Rx : 3


//-----------------온습도용
#include <DHT.h>
#define DHTPIN 5  //배변 온습도용 
#define DHTPIN2 6 //날씨 온습도용
#define DHTTYPE DHT11
#define DHTTYPE2 DHT11
DHT dht(DHTPIN, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE2);
unsigned long time_previous, time_current;
float sum_T, cnt_T;
float sum_S, cnt_S;
float avg_T,avg_S;
int bt;

float uv;
float dust;
// --------------

String mode;
char myChar;
String temp; //복사용
void setup(){

  
  
  Serial.begin(9600);                   // 시리얼 통신을 사용하기 위해 보드레이트를 9600으로 설정
  pinMode(ReadUVintensityPin, INPUT);//자외선
  BTSerial.begin(9600);
  pinMode(ledPower,OUTPUT); // 필요없는거
  Wire.begin(9600);
  bufferPosition = 0;

}





 
void loop(){
       
     
       delay(5000);
       float a = dht.readTemperature();
       float b = dht.readHumidity(); 
       float c = dht2.readTemperature();  // 외부 온도 용 ( 선풍기 )
       float d = dht2.readHumidity();     // 외부 습도 용 ( 선풍기 ) 
        
       float uv = uvCheck();
       float dust = dustCheck();

       int new_c = c*100;
       int new_d = d*100;
       
       int new_uv = uv*100;
       int new_dust = dust*100;
       
       start(a,b,c,d,uv,dust); // 어플에 센서 값 전송
    
    mode ="";   // 받는 값 초기화
     
    while(BTSerial.available()){
      char myChar = (char)BTSerial.read();
      mode += myChar;
      if(myChar =='\n'){
        temp = mode;
        break;
      } // 문자열 받기 완료
    }// 문자열 받기 완료

    
     BTSerial.print("temp: ");
     BTSerial.print(temp);
     BTSerial.print("\n");
     BTSerial.print("MOde: ");
     BTSerial.print(mode);
     BTSerial.print("\n");

//    문자열 인덱스  
//    0:가림막 모드(a: 자동, p: 수동)
//    1:닫기 or 열기 (0이 p일때, o: 열기, c: 닫기)
//    2:선풍기 모드(a: 자동, p: 수동)
//    3:회전 or off (2가 p일때, o: 회전, c : off)
   

    if (temp[0] =='a') // 자동화
    {
    
    Wire.beginTransmission(sv2);
    Wire.write(new_uv);
    Wire.write(new_dust);
    Wire.write('a');  // 자동화 알려주는거
    Wire.endTransmission();
    
    }

    else if (temp[0] =='p' && temp[1] == 'o')  // 수동화 열기   po
    {
      
      Wire.beginTransmission(sv2);
      Wire.write(0);  //일단 쓰레기 값
      Wire.write(0);  //일단 쓰레기 값
      Wire.write("o");  // 수동 열라고 알려주기
      Wire.endTransmission();
      
    }
    
    else if (temp[0]=='p' && temp[1] =='c')  // 수동화 닫기     pc 
    {  
      
      Wire.beginTransmission(sv2);
      Wire.write(0);  //일단 쓰레기 값
      Wire.write(0);  //일단 쓰레기 값
      Wire.write("c");  // 수동 닫으라고 알려주기
      Wire.endTransmission();
      
    }

    if (temp[2] =='a')  //선풍기 자동 모드
    {
      
      Wire.beginTransmission(sv1);
      Wire.write(int(c));
      Wire.write(int(d));
      Wire.write("a");  // 선풍기 자동이라고 알려주기
      Wire.endTransmission();
      
    }
    else if(temp[2] =='p' && temp[3] == 'o') // 수동 회전
    {
      
      Wire.beginTransmission(sv1);
      Wire.write(0);  //일단 쓰레기 값
      Wire.write(0);  //일단 쓰레기 값
      Wire.write("o"); // 선풍기 회전 알려주기
      Wire.endTransmission();
      
    }

    else if(temp[2] =='p' && temp[3] =='c') //수동 off
    {
      
      Wire.beginTransmission(sv1);
      Wire.write(0);  //일단 쓰레기 값
      Wire.write(0);  //일단 쓰레기 값
      Wire.write("c");   // 선풍기 off 알려주기
      Wire.endTransmission();
      
    }
    
    
    
    
}// loop end


 



 void start(float a, float b, float c, float d, float uv, float dust){
       

        

       BTSerial.print("먼지:");
       BTSerial.print(dust);                              // dustDensity을 시리얼 통신으로 출력합니다.
       BTSerial.print(" ug/m3 입니다.");                          // " ug/m3"를 시리얼 통신으로 출력하고 줄을 바꿉니다.
       BTSerial.print(" 자외선: ");
       BTSerial.print(uv);
       BTSerial.print(" mW/cm^2 입니다.  ");
       BTSerial.print("온습도용 온도는 ");
       BTSerial.print(a);
       BTSerial.print(" 이랑 ");
       BTSerial.print(b);
       BTSerial.print(" 입니다 ");
       BTSerial.print("날씨용 온습도는 ");
       BTSerial.print(c);
       BTSerial.print("이랑 ");
       BTSerial.print(d);
       BTSerial.println(" 입니다. ");
       
}






//----------------센싱용
















float dustCheck(){
  digitalWrite(ledPower,LOW);                               // ledPower를 LOW로 설정합니다.


  
  delayMicroseconds(samplingTime);                    // samplingTime(280) 마이크로초만큼 지연합니다.
  voMeasured = analogRead(measurePin);             //  measurePin의 아날로그 값을 받아 voMeasured에 저장합니다.
 
  delayMicroseconds(deltaTime);                          // deltaTime(40) 마이크로초만큼 지연합니다.
  digitalWrite(ledPower,HIGH);                              // ledPower를 HIGH로 설정합니다.
  delayMicroseconds(sleepTime);                        // sleepTime(9680) 마이크로초만큼 지연합니다.
 
  calcVoltage = voMeasured * (5.0 / 1024.0);         // voMeasured의 값을 5.0/1024.0을 곱하여 calcVoltage에 저장합니다.
 
  dustDensity = (0.17 * calcVoltage - 0.1) * 1000;   // calcVoltage 값에 0.17을 곱하고 -0.1을 더합니다. (mg/m3)
                                                   // 값을 ug/m3 단위로 표현하기 위해 1000을 곱하여 dustDensity에 저장하여줍니다.
  return (dustDensity);
}





float uvCheck(){
  

  int uvLevel = averageAnalogRead(ReadUVintensityPin);
  float outputVoltage = 5.0 * uvLevel/1024; //전압을 아날로그 신호 범위로 변환
  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.9, 0.0, 15.0); //UV 강도로 변환
  return(uvIntensity);
}

int averageAnalogRead(int pinToRead) //uv
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 
 
  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;
 
  return(runningValue);  
 
}
 
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) //uv
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}