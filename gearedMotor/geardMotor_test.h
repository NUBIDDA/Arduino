int ENA = 10; 
int INA1 = 9; 
int INA2 = 8; 
int cmd = 0; 

void setup() 
{ 
  Serial.begin(9600); 
  pinMode(ENA, OUTPUT); 
  pinMode(INA1, OUTPUT); 
  pinMode(INA2, OUTPUT); 
  digitalWrite(ENA, LOW); 
  digitalWrite(INA1, LOW); 
  digitalWrite(INA2, LOW); 
  Serial.println("DC motor test");
  Serial.println("Forward "); 
  digitalWrite(ENA, HIGH);
  digitalWrite(INA1, HIGH); 
  digitalWrite(INA2, LOW); 
  delay(3000); 
  Serial.println("Backward"); 
  digitalWrite(ENA, HIGH);
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, HIGH); 
  delay(3000); 
  Serial.println("stop"); 
  digitalWrite(ENA, LOW); 
  digitalWrite(INA1, LOW); 
  digitalWrite(INA2, LOW); 
  delay(1000); 
  Serial.println("Forward-PWM"); 
  analogWrite(ENA, 150); 
  digitalWrite(INA1, HIGH); 
  digitalWrite(INA2, LOW); 
  delay(3000); 
  Serial.println("Backward-PWM");
  analogWrite(ENA, 150); 
  digitalWrite(INA1, LOW); 
  digitalWrite(INA2, HIGH); 
  delay(3000); 
  Serial.println("stop");
  digitalWrite(ENA, LOW);
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, LOW);
  } 
  void loop() { }


