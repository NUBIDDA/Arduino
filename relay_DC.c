const int pinRelay = 3;
byte c = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pinRelay, OUTPUT);
}

void loop() {
  while (Serial.available()) {
    c = Serial.read();
    Serial.println(c);

    if (c == '1') {
      digitalWrite(pinRelay, HIGH);
      Serial.println("1channel Realy ON");
    } else {
      digitalWrite(pinRelay, LOW);
      Serial.println("1channel Relay OFF");
    }
  }
}