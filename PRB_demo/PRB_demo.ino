
//for following condition:
//(pin 17)Test = 1; (pin 18,19,20) => LBEN^ = HBEN^ = CE^/LOAD^ = 0;

byte b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,Pol,Org; 
long dec=0; byte state;
const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average



void setup() {
  pinMode(A0, INPUT);pinMode(9, INPUT); pinMode(A2, INPUT);pinMode(A3, INPUT); 
  pinMode(A4, INPUT);pinMode(A5, INPUT); pinMode(A6, INPUT);pinMode(A7, INPUT);
  pinMode(A8, INPUT);pinMode(A9, INPUT); pinMode(A10, INPUT);pinMode(A11, INPUT);
  pinMode(A12, INPUT);pinMode(A13, INPUT); pinMode(A14, INPUT);pinMode(A15, INPUT);
  pinMode(8, OUTPUT);pinMode(9, OUTPUT);
  delay(1);
  Serial.begin(9600); delay(10);
   for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop() {
  //digitalWrite(9, HIGH); //hben pin19
  //digitalWrite(8, HIGH); //lben pin18
  state = digitalRead(A15);
if (state == 0) {
  //Serial.print("\n Status: "); Serial.print(digitalRead(state));
  //digitalWrite(8, LOW); digitalWrite(9, LOW);
  b1 =digitalRead(A0);b2 =digitalRead(A1);b3 =digitalRead(A2);b4 =digitalRead(A3);
  b5 =digitalRead(A4);b6 =digitalRead(A5);b7 =digitalRead(A6);b8 =digitalRead(A7); 
  //delay(1);
  //digitalWrite(9, HIGH); delay(10); digitalWrite(9, LOW);
  b9 =digitalRead(A8);b10 =digitalRead(A9);b11 =digitalRead(A10);b12 =digitalRead(A11);
  Org =digitalRead(A12); Pol =digitalRead(A13); //delay(10);
  //digitalWrite(9, HIGH); digitalWrite(8, HIGH);

  dec= b1*(pow(2,0))+b2*(pow(2,1))+b3*(pow(2,2))+b4*(pow(2,3))+b5*(pow(2,4))+b6*(pow(2,5))+
        b7*(pow(2,6))+b8*(pow(2,7))+b9*(pow(2,8))+b10*(pow(2,9))+b11*(pow(2,10))+b12*(pow(2,11));

   //Serial.print(" BIN:  ");         
   //Serial.print(b1);/*Serial.print(", ");*/ Serial.print(b2);/*Serial.print(", ");*/ Serial.print(b3);Serial.print(", ");
   //Serial.print(b4);/*Serial.print(", ");*/ Serial.print(b5);/*Serial.print(", ");*/ Serial.print(b6);Serial.print(", ");
   //Serial.print(b7);/*Serial.print(", "); */Serial.print(b8);/*Serial.print(", "); */Serial.print(b9);Serial.print(", ");
   //Serial.print(b10);/*Serial.print(", ");*/Serial.print(b11);/*Serial.print(", ");*/Serial.print(b12);
   //Serial.print(" POL: "); Serial.print(Pol);Serial.print(" ORG: "); Serial.print(Org);
   //Serial.print("  Decimal Count: ");
   if(Pol==0){ dec= 5000-dec;} else {dec=5000+dec;}
   Serial.print(dec); //Serial.print("\t");
   if(Org==1){ Serial.println("^"); } else {Serial.println("~"); }

//  total = total - readings[readIndex];
//  readings[readIndex] = dec;
//  total = total + readings[readIndex];
//  readIndex = readIndex + 1;
//
//  if (readIndex >= numReadings) {
//    readIndex = 0;
//  }
//  average = total / numReadings;
//  Serial.println(average);
 }
else  return(0); 
}                                                                          
