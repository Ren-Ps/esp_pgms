
//for following condition:
//(pin 17)Test = 1; (pin 18,19,20) => LBEN^ = HBEN^ = CE^/LOAD^ = 0;

int b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,Pol,Org; 
int dec=0; byte stat;

void setup() {
  pinMode(0, INPUT);pinMode(1, INPUT); pinMode(2, OUTPUT);pinMode(3, OUTPUT); 
  pinMode(4, INPUT);pinMode(5, INPUT); pinMode(6, INPUT);pinMode(7, INPUT);
  pinMode(8, INPUT);pinMode(9, INPUT); pinMode(10, INPUT);pinMode(11, INPUT);
  pinMode(12, INPUT);pinMode(13, INPUT);              //pinMode(A2, OUTPUT); pinMode(A1, OUTPUT); pinMode(A0, INPUT);
  delay(1);
  
  Serial.begin(9600); delay(10);
}

void loop() {
//  digitalWrite(2, HIGH); //hben
//  digitalWrite(3, HIGH); //lben
  
//if (digitalRead(12) == 0) {
  Serial.print("\n Status: "); Serial.print(digitalRead(12));
  
  digitalWrite(3, LOW);  digitalWrite(2, LOW);// FIRST READ LOWER BYTES
  b1 =digitalRead(4);b2 =digitalRead(5);b3 =digitalRead(6);b4 =digitalRead(7); 
  b5 =digitalRead(8);b6 =digitalRead(9);b7 =digitalRead(10);b8 =digitalRead(11); 
  
//  digitalWrite(3, HIGH); delay(1); digitalWrite(2, LOW); //THEN HIGHER BYTES
//  b9 =digitalRead(4);b10 =digitalRead(5);b11 =digitalRead(6);b12 =digitalRead(7);
//  Org =digitalRead(8); Pol =digitalRead(9); delay(1);
//
//  digitalWrite(2, HIGH); //digitalWrite(A2, HIGH);
//
//  dec= b1*(pow(2,0))+b2*(pow(2,1))+b3*(pow(2,2))+b4*(pow(2,3))+b5*(pow(2,4))+b6*(pow(2,5))+
//        b7*(pow(2,6))+b8*(pow(2,7))+b9*(pow(2,8))+b10*(pow(2,9))+b11*(pow(2,10))+b12*(pow(2,11));
//
   Serial.print(" BIN:  ");         
   Serial.print(b1);/*Serial.print(", ");*/ Serial.print(b2);/*Serial.print(", ");*/ Serial.print(b3);Serial.print(", ");
   Serial.print(b4);/*Serial.print(", ");*/ Serial.print(b5);/*Serial.print(", ");*/ Serial.print(b6);Serial.print(", ");
   Serial.print(b7);/*Serial.print(", "); */Serial.print(b8);/*Serial.print(", "); */Serial.print(b9);Serial.print(", ");
//   Serial.print(b10);/*Serial.print(", ");*/Serial.print(b11);/*Serial.print(", ");*/Serial.print(b12);
//   Serial.print(" POL: "); Serial.print(Pol);Serial.print(" ORG: "); Serial.print(Org);
//   Serial.print("  Decimal Count: "); Serial.print(dec); Serial.print(" "); delay(10);
//  }
//else  return(0); 
delay(10);
}
