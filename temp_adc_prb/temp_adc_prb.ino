void setup() {
  // put your setup code here, to run once:
//pinMode(A6, INPUT);

Serial.print(9600);
delay(1);
}

void loop() {
  // put your main code here, to run repeatedly:

int count= analogRead(A6);
delay(1);
Serial.println(count);

}
