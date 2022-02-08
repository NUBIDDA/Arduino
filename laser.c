int laserPin = 13;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9660);
  pinMode(laserPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(laserPin, HIGH);
  delay(1000);
  digitalWrite(laserPin, LOW);
  delay(1000);
}