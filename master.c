
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
float uv;
float dust;
// --


SoftwareSerial BTSerial(2, 3); //Tx :2 Rx : 3


//-----------------온습도
#include <DHT.h>
#define DHTPIN 5  //배변 온습도용 
#define DHTPIN2 6 //날씨 온습도용
#define DHTTYPE DHT11
#define DHTTYPE2 DHT11
DHT dht(DHTPIN, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE2);
unsigned long time_previous, time_current, time_previous2, time_current2;
float sum_T, cnt_T;   // 배변용
float sum_S, cnt_S;   // 배변용
float avg_T,avg_S;    // 배변용
int bt;


// --------------

String mode;
char myChar;
String temp; //복사용
char sheet_mode;


void setup(){

  
  
  Serial.begin(9600);                   // 시리얼 통신을 사용하기 위해 보드레이트를 9600으로 설정
  pinMode(ReadUVintensityPin, INPUT);//자외선
  BTSerial.begin(9600);
  pinMode(ledPower,OUTPUT); // 필요없는거
  Wire.begin(9600);
 


  
  unsigned long time_previous =0;
  unsigned long time_previous2 =0;
  sum_T = 0;
  sum_S = 0;
  avg_T = 0;
  avg_S = 0;
  cnt_T = 0;
  cnt_S = 0;
//  int interval = 60000;
  

}





 
void loop(){

       time_current = millis();
       time_current2 =millis();
       sheet_mode = 'x';
       
       float a = dht.readTemperature();  // 배변 용 온도
       float b = dht.readHumidity();     // 배변용 습도

       if(a > 0 && b > 0){
           sum_T = sum_T + a ;
           cnt_T ++;

           sum_S = sum_S + b ;
           cnt_S ++;
        }

       if(time_current - time_previous >=60000) {
          time_previous = time_current;
          
          Serial.println("10초가 지났습니다");
          
          avg_T = sum_T / cnt_T ;
          avg_S = sum_S / cnt_S ;
          sum_T = 0;
          cnt_T = 0;
          sum_S = 0;
          cnt_S = 0; //초기화
          Serial.println("측정을 시작합니다");
          Serial.println("평균과 비교합니다");
          Serial.print("평균 온도는 ");
          Serial.print(avg_T);
          Serial.print("  평균 습도는 ");
          Serial.println(avg_S);

       }
    
       
       
      
       if(time_current2 - time_previous2 >=60000){
       if (a > (avg_T+3)  && b > (avg_S+3) ){
            
          sheet_mode='o';
       }
        
       }

        Serial.print("현재 온도는 ");
       Serial.print(a);
       Serial.print("현재 습도는 ");
       Serial.println(b);
       Serial.print("배변활동 은 ");
       Serial.println(sheet_mode);
       
       float c = dht2.readTemperature();  // 외부 온도 용 ( 선풍기 )
       float d = dht2.readHumidity();     // 외부 습도 용 ( 선풍기 ) 
        
       float uv = uvCheck();
       float dust = dustCheck();

       // float  -> int -> float

       float float_c = c*100;
       int int_temp = int(float_c);

       float float_d = d*100;
       int int_humi = int(float_d);

       float float_uv = uv*100;
       int int_uv = int(float_uv);

       float float_dust = dust*100;
       int int_dust = int(float_dust);
     
//       float new_c = float(int_c)/100;  << 이건 slave에서


   
       start(a,b,c,d,uv,dust,sheet_mode); // 어플에 센서 값 전송
    
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
        BTSerial.print("Mode: ");
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
    Wire.write(int_uv);
    Wire.write(int_dust);
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
      Wire.write(int_temp);
      Wire.write(int_humi);
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
    
    delay(5000);
    
    
}// loop end


 



 void start(float a, float b, float c, float d, float uv, float dust, char sheet_mode){
       

        

       BTSerial.print("먼지:");
       BTSerial.print(dust);                             
       BTSerial.println(" ug/m3 입니다.");                         
       BTSerial.print(" 자외선: ");
       BTSerial.print(uv);
       BTSerial.println(" mW/cm^2 입니다.  ");
       BTSerial.print("온습도용 온도는 ");
       BTSerial.print(a);
       BTSerial.print(" 이랑 ");
       BTSerial.print(b);
       BTSerial.println(" 입니다 ");
       BTSerial.print("날씨용 온습도는 ");
       BTSerial.print(c);
       BTSerial.print("이랑 ");
       BTSerial.print(d);
       BTSerial.println(" 입니다. ");
       BTSerial.print("배변활동 o/x는 ");
       BTSerial.println(sheet_mode);  // 여기까지 테스트용
       String app =String(dust) + ',' + String(uv) + ',' + String(c) + ',' + String(d) + ',' + String(sheet_mode);
       // 어플이 쓸 문자열
       BTSerial.println(app);
       
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
