
//for following condition:
//(pin 17)Test = 1; (pin 18,19,20) => LBEN^ = HBEN^ = CE^/LOAD^ = 0;

int b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,Pol,Org; 
int dec=0; byte stat;

void setup() {
  /*pinMode(0, INPUT);pinMode(1, INPUT);*/ pinMode(2, INPUT);pinMode(3, INPUT); 
  pinMode(4, INPUT);pinMode(5, INPUT); pinMode(6, INPUT);pinMode(7, INPUT);
  pinMode(8, INPUT);pinMode(9, INPUT); pinMode(10, INPUT);pinMode(11, INPUT);
  pinMode(12, INPUT);pinMode(13, INPUT); pinMode(A0, INPUT); pinMode(A1, INPUT);
  pinMode(A2, INPUT);pinMode(A2, INPUT);pinMode(A3, INPUT);pinMode(A4, INPUT);
  pinMode(A5, OUTPUT);pinMode(A6, OUTPUT);pinMode(A7, OUTPUT);
  
  delay(1);
  Serial.begin(9600); delay(10);
  digitalWrite(A5, LOW);digitalWrite(A6, LOW);digitalWrite(A7, LOW);
}

void loop() {
//  digitalWrite(A1, HIGH); //hben
//  digitalWrite(A2, HIGH); //lben

digitalWrite(A5, LOW);digitalWrite(A6, LOW);digitalWrite(A7, LOW);

if (digitalRead(A2) == 0) {
  Serial.print("\n Status: "); Serial.print(digitalRead(A2));
  //digitalWrite(A2, LOW);
  
  b1 =digitalRead(2);b2 =digitalRead(3);b3 =digitalRead(4);b4 =digitalRead(5);
  b5 =digitalRead(6);b6 =digitalRead(7);b7 =digitalRead(8);b8 =digitalRead(9); //
  
  //digitalWrite(A2, HIGH); delay(10); digitalWrite(A1, LOW);
  b9 =digitalRead(10);b10 =digitalRead(11);b11 =digitalRead(12);b12 =digitalRead(13);
  Org =digitalRead(A0); Pol =digitalRead(A1); delay(10);
  
 // digitalWrite(A1, HIGH); digitalWrite(A2, HIGH);

  dec= b1*(pow(2,0))+b2*(pow(2,1))+b3*(pow(2,2))+b4*(pow(2,3))+b5*(pow(2,4))+b6*(pow(2,5))+
        b7*(pow(2,6))+b8*(pow(2,7))+b9*(pow(2,8))+b10*(pow(2,9))+b11*(pow(2,10))+b12*(pow(2,11));

   Serial.print(" BIN:  ");         
   Serial.print(b1);/*Serial.print(", ");*/ Serial.print(b2);/*Serial.print(", ");*/ Serial.print(b3);Serial.print(", ");
   Serial.print(b4);/*Serial.print(", ");*/ Serial.print(b5);/*Serial.print(", ");*/ Serial.print(b6);Serial.print(", ");
   Serial.print(b7);/*Serial.print(", "); */Serial.print(b8);/*Serial.print(", "); */Serial.print(b9);Serial.print(", ");
   Serial.print(b10);/*Serial.print(", ");*/Serial.print(b11);/*Serial.print(", ");*/Serial.print(b12);
   Serial.print(" POL: "); Serial.print(Pol);Serial.print(" ORG: "); Serial.print(Org);
   Serial.print("  Decimal Count: "); Serial.print(dec); Serial.print(" ");
  }
else  return(0); 
}
