void setup() {
  // put your setup code here, to run once:
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  pinMode(D0, OUTPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("D5_H"); digitalWrite(D5, HIGH); delay(1000); digitalWrite(D5, LOW); delay(50);
  Serial.println("D6_H"); digitalWrite(D6, HIGH); delay(1000); digitalWrite(D6, LOW); delay(50);
  Serial.println("D7_H"); digitalWrite(D7, HIGH); delay(1000); digitalWrite(D7, LOW); delay(50);
  Serial.println("D8_H"); digitalWrite(D8, HIGH); delay(1000); digitalWrite(D8, LOW); delay(50);

  digitalWrite(D0, HIGH); delay(500); digitalWrite(D0, LOW); delay(500);
  digitalWrite(D5, LOW); delay(100);
  digitalWrite(D6, LOW);  delay(200);
  digitalWrite(D7, LOW);  delay(300);
  digitalWrite(D8, LOW); delay(400);
  digitalWrite(D0, HIGH); delay(500); digitalWrite(D0, LOW); delay(500);

  digitalWrite(D5, HIGH); delay(100);
  digitalWrite(D6, HIGH);  delay(200);
  digitalWrite(D7, HIGH);  delay(300);
  digitalWrite(D8, HIGH); delay(400);

}
