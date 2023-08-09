void setup() {
  Serial.begin(19200); Serial.setTimeout(100);
  delay(5000);
  Serial.println("DATA001       0.00 +NG "); 
  Serial.println("DATA002       0.2 +NG "); delay(1000); 
  Serial.println("DATA003       -0.2 +NG ");
  Serial.println("DATA004        0.0 OK1 "); delay(1000); 
  Serial.println("DATA005     -0.2 -NG "); 
  Serial.println("DATA006     -0.2 -NG "); delay(1000); 
  Serial.println("DATA007      0.2 +NG ");
  Serial.println("DATA008       0.2 +NG "); delay(1000); 
  Serial.println("DATA009      -0.2 -NG "); delay(1000); 
  Serial.println("DATA010       0.2 +NG "); 

   delay(5000);
  Serial.println("DATA001       -0.10 +NG "); 
  Serial.println("DATA002       0.1 +NG "); delay(1000); 
  Serial.println("DATA003       -0.1 +NG ");
  Serial.println("DATA004        0.0 OK1 "); delay(1000); 
  Serial.println("DATA005     -0.1 -NG "); 
  Serial.println("DATA006     0.1 -NG "); delay(1000); 
  Serial.println("DATA007     0.1 +NG ");
  Serial.println("DATA008      -0.1 +NG "); delay(1000); 
  Serial.println("DATA009      -0.1 -NG "); delay(1000); 
  Serial.println("DATA010       0.1 +NG "); 

}

void loop() {
  // put your main code here, to run repeatedly:

}
