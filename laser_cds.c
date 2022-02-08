int laserPin = 13;  // 레이저 모듈
void setup() {
  Serial.begin(9600);
  pinMode(laserPin, OUTPUT);
}

void loop() {
  int cds = analogRead(A0);  // 조도 센서 (CDS)
  digitalWrite(laserPin, HIGH);  // 레이저 모듈에서 빛이 계속 나오는 상태
  Serial.println(cds);
  if(cds < 900) {  // 사용자의 손으로 인해 빛이 차단되었다면,
    Serial.println("Using");
  } else {  // 빛이 차단되지 않고 조도 센서에 많은 양의 빛이 들어온다면,
    Serial.println("Not Using");
  }
  delay(1000);
}