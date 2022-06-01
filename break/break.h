int laserPin = 8;  // 레이저 모듈
int sensor = 4;  // 솔레노이드

void setup() {
  Serial.begin(9600);
  pinMode(laserPin, OUTPUT);
  pinMode(sensor, OUTPUT);
  
}
` 
void loop() {
  int cds = analogRead(A0);  // 조도 센서 (CDS)
  digitalWrite(laserPin, HIGH);  // 레이저 모듈에서 빛이 계속  ` 나오는 상태
  Serial.println(cds);
  if(cds < 900) {  // 사용자의 손으로 인해 빛이 차단되었다면,
    Serial.println("Using");
    digitalWrite(sensor, LOW);  // 브레이크(솔레노이드) 해제
  } else {  // 빛이 차단되지 않고 조도 센서에 많은 양의 빛이 들어온다면,
    Serial.println("Not Using");
    digitalWrite(sensor, HIGH);  // 브레이크 (솔레노이드) 작동
  }
  delay(1000);
}

// 추후 사용자가 잠깐 손을 땠을 경우를 대비한 예외 코드 작성 필요