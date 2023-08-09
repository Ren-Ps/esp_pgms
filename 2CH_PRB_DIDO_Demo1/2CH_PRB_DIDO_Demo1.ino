/* 

   This Code contains Selft Decleared and developed program
   This code is in the Public Domain (or CC0 licensed, at programmer's option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
***********************************************************************************************
   @Ps, 15 Mar, 2019
    ALL COPY AND RESERVED RIGHTS HELD,  ©PS..
***********************************************************************************************
          _____   _____  
    ____ |  __ \ / ____| 
   / __ \| |__) | (___   
  / / _` |  ___/ \___ \  
 | | (_| | |     ____) | 
  \ \__,_|_|    |_____(_)
   \____/                
   
*/

byte CH0[13]={0};  byte CH1[13]={0}; byte CH2[13]={0};  byte CH3[13]={0};
byte Pol[5]={0};   byte Org[5]={0};
long dec[5]={0};  boolean stat=1;

void setup() {
  for ( int i=22; i<54; i++){
    pinMode(i,INPUT);
  }
  Serial.begin(9600); Serial.setTimeout(100); delay(10);
  Serial1.begin(9600); Serial1.setTimeout(100); delay(10);
  delay(10);
}

void loop() {
  
  if ((digitalRead(53) == 1)){ stat=1; }
  
  if (stat==1 && (digitalRead(53) == 0)) {
  /*CH0[1] =digitalRead(25);  CH0[2] =digitalRead(27);  CH0[3] =digitalRead(29);  CH0[4] =digitalRead(31);
  CH0[5] =digitalRead(33);  CH0[6] =digitalRead(35);  CH0[7] =digitalRead(37);  CH0[8] =digitalRead(39); 
  CH0[9] =digitalRead(41);  CH0[10] =digitalRead(43); CH0[11] =digitalRead(45);  CH0[12] =digitalRead(47);*/
  Org[1] =digitalRead(49); Pol[1] =digitalRead(51); 
  
  dec[1]= digitalRead(25)*(pow(2,0))+digitalRead(27)*(pow(2,1))+digitalRead(29)*(pow(2,2))+digitalRead(31)*(pow(2,3))+digitalRead(33)*(pow(2,4))+
          digitalRead(35)*(pow(2,5))+digitalRead(37)*(pow(2,6))+digitalRead(39)*(pow(2,7))+digitalRead(41)*(pow(2,8))+digitalRead(43)*(pow(2,9))+
          digitalRead(45)*(pow(2,10))+digitalRead(47)*(pow(2,11));
   
  // Serial.print(" CH01");
   if(Pol[1]==0){ dec[1] = 5000-dec[1];} else {dec[1]=5000+dec[1];}
   /*Serial.print(dec[1]);
   //if(Org[1]==1){ Serial.print("^"); } else {Serial.print("~"); };  stat=0;*/
 
  if (digitalRead(52) == 0) {
  /*CH0[1] =digitalRead(24);  CH0[2] =digitalRead(26);  CH0[3] =digitalRead(28);  CH0[4] =digitalRead(30);
  CH0[5] =digitalRead(32);  CH0[6] =digitalRead(34);  CH0[7] =digitalRead(36);  CH0[8] =digitalRead(38); 
  CH0[9] =digitalRead(40);  CH0[10] =digitalRead(42); CH0[11] =digitalRead(44);  CH0[12] =digitalRead(46);*/
  Org[2] =digitalRead(48); Pol[2] =digitalRead(50); 
  
  dec[2]= digitalRead(24)*(pow(2,0))+digitalRead(26)*(pow(2,1))+digitalRead(28)*(pow(2,2))+digitalRead(30)*(pow(2,3))+digitalRead(32)*(pow(2,4))+
          digitalRead(34)*(pow(2,5))+digitalRead(36)*(pow(2,6))+digitalRead(38)*(pow(2,7))+digitalRead(40)*(pow(2,8))+digitalRead(42)*(pow(2,9))+
          digitalRead(44)*(pow(2,10))+digitalRead(46)*(pow(2,11));
   
   //Serial.print(", CH02");
   if(Pol[2]==0){ dec[2] = 15000+dec[2];} else {dec[2]=15000-dec[2];}
   /*Serial.print(dec[2]);
   //if(Org[2]==1){ Serial.println("^"); } else {Serial.println("~"); } //delay(10);*/
    } 
  }
  Serial.print("CH01-");Serial.print(dec[1]);Serial.print(", CH02-");Serial.print(dec[2]);Serial.print(".\n\r");
  Serial.flush();
  if (Serial.available()) {
    String str = Serial.readStringUntil('\n');
    Serial1.print(str); Serial1.print("\n\r");
  } Serial1.flush();
  Serial1.print("CH01-");Serial1.print(dec[1]);Serial1.print(", CH02-");Serial1.print(dec[2]);Serial1.print(".\n\r");
  if (Serial1.available()) {
    String str = Serial1.readStringUntil('\n');
    Serial.print(str); Serial.print("\n\r");
  } Serial.flush();
}                                                                          
