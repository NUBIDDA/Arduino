#include <DHT.h>
#include <SoftwareSerial.h>
#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
unsigned long time_previous, time_current;
float sum_T, cnt_T;
float sum_S, cnt_S;
float avg_T,avg_S;
int bt;



SoftwareSerial BTSerial(2,3);   //bluetooth module Tx:Digital 2 Rx:Digital 3

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13,LOW);
  
  Serial.begin(9600);
  BTSerial.begin(9600);

  unsigned long time_previous =0;
  sum_T = 0;
  sum_S = 0;
  avg_T = 0;
  avg_S = 0;
  cnt_T = 0;
  cnt_S = 0;
}

void loop() {
  
  bt=3;
  delay(10000);
  
  if (BTSerial.available())
  {

   
    bt = BTSerial.read();
    if(bt =='1'){
      BTSerial.println("10초간 온도값을 재겠습니다.");
      while(bt=='1'){
      
        time_current  = millis();
        float a = dht.readTemperature();
        float b = dht.readHumidity();
        Serial.println(a);
        Serial.println(b);
        BTSerial.print("온도: ");
        BTSerial.print(a);
        BTSerial.print(" C ");
        BTSerial.print("습도: ");
        BTSerial.println(b);
        if(a > 0 && b > 0){
        sum_T = sum_T + a ;
        cnt_T ++;

        sum_S = sum_S + b ;
        cnt_S ++;
        }
        
        delay(1000);
//1누르면 시작
       

        
      if(time_current - time_previous >=30000) {
        time_previous = time_current;
        BTSerial.println("10초가 지났습니다");
        
        avg_T = sum_T / cnt_T ;
        avg_S = sum_S / cnt_S ;
        
        BTSerial.print("평균 온도는 ");
        BTSerial.print(avg_T);
        BTSerial.print("  평균 습도는 ");
        BTSerial.println(avg_S);
        bt =2;
        
      }
       
      }
      BTSerial.println("측정을 시작합니다");
      BTSerial.println("평균과 비교합니다");
      while(1){
        
        float d = dht.readTemperature();
        float e = dht.readHumidity();
        BTSerial.print(" 평균은");
        BTSerial.print(avg_T);
        BTSerial.print(" , ");
        BTSerial.print(avg_S);
        
        BTSerial.print(" 현재는 ");
        BTSerial.print(d);
        BTSerial.print(" , ");
        BTSerial.println(e);
        
        delay(1000);

        if(d > (avg_T + 3) && e > (avg_S + 3)){
          BTSerial.println("배변활동이 의심됩니다");
          break;
        }
      }
    
    } // 1값 받은거 끝
  
  } // available() 끝
} // loop 끝

  
 