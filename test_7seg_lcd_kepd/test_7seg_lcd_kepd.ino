#include <LiquidCrystal.h>
#include <Keypad.h>
#include <EEPROM.h>

struct master {
  unsigned long M1, M2, ASV, USL, LSL; };

struct master arry_mstr[30];

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

byte rowPins[ROWS] = { 13, 12, 11, 10 }; //connect to the row pinouts of the keypad
byte colPins[COLS] = { 9, 8, 7, 6 }; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const int rs = 25, en = 26, d4 = 27, d5 = 28, d6 = 29, d7 = 30;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define din 22
#define cs 23
#define clk 24

byte flag=0; byte flag0=0;

//void initialise(){
//  pinMode(din, OUTPUT);
//  pinMode(cs, OUTPUT);
//  pinMode(clk, OUTPUT);
//}

//void out (byte address, byte data){
//  digitalWrite(cs, LOW);
//  shiftOut(din, clk, MSBFIRST, address);
//  shiftOut(din, clk, MSBFIRST, data);
//  digitalWrite(cs, HIGH);
//}

void setup() {
  Serial.begin(9600);
  
//  initialise();
//    
//  out(0x0c, 0x00); delay(200);
//  out(0x0c, 0x01);
//  out(0x0a, 0x0f); //intensity
//  out(0x0f, 0x01); delay(100); //test
//  out(0x0f, 0x00); 
//  out(0x0b, 0x07); //number of digits enabled
//  out(0x09, 0x00); delay(100); //decoder mode off
//      //PS
//  out(0x01, 0x00);
//  out(0x02, 0x00);
//  out(0x03, 0x59);
//  out(0x04, 0x4b);
//  out(0x05, 0x61);
//  out(0x06, 0x47);
//  out(0x07, 0x00);
//  out(0x08, 0x00); delay (503);
//      //HELLO !
//  out(0x01, 0xb0);
//  out(0x02, 0x00);
//  out(0x03, 0x7e);
//  out(0x04, 0x0e);
//  out(0x05, 0x0e);
//  out(0x06, 0x4f);
//  out(0x07, 0x37);
//  out(0x08, 0x00); delay (100);
//      //MICRO
//  out(0x01, 0x00);
//  out(0x02, 0x7e);
//  out(0x03, 0x77);
//  out(0x04, 0x4e);
//  out(0x05, 0x30);
//  out(0x06, 0x72);
//  out(0x07, 0x66);
//  out(0x08, 0x00); delay (100);
//      //TECH
//  out(0x01, 0x00);
//  out(0x02, 0x80);
//  out(0x03, 0x37);
//  out(0x04, 0x4e);
//  out(0x05, 0x4f);
//  out(0x06, 0x46);
//  out(0x07, 0x70);
//  out(0x08, 0x00); delay (100);

//  //decoder mode on and starts,
//  out(0x09, 0xff);
//  for (int i=0; i<9; i++){
//    out(i, 0);
//  }
//
//  for( int i=0; i<=8; i++){
//    for( int j=0; j<=9; j++){
//    out(i, j); delay(200);
//    }
//  } delay(100);

//  //PROBE
//  out(0x09, 0x00);
//  out(0x08, 0x67);
//  out(0x07, 0x77);
//  out(0x06, 0x7e);
//  out(0x05, 0x7f); delay(100);
//  out(0x09, 0x0f);
//  out(0x04, 0x0b);
//  out(0x03, 0x0a);
//  out(0x02, 0x02);
//  out(0x01, 0x02); delay(100);
//  
//  //Reading example
//  out(0x09, 0xff);
//  out(0x08, 0x01);
//  out(0x07, 0x02);
//  out(0x06, 0x03);
//  out(0x05, 0x04);
//  out(0x04, 0x85);
//  out(0x03, 0x06);
//  out(0x02, 0x07);
//  out(0x01, 0x08);
  
  lcd.begin(16, 2);
  lcd.print("hello, world!"); delay(200); Serial.println("Hello..!");
}

void loop() {
  String numb="";
  
  if (flag==0){
    char key0 = keypad.getKey();
  switch(key0){
    case 'A' :  lcd.clear();                //Serial.println("Master Mode: ");
                lcd.print("Master Mode:");
                flag=1; key0=""; break;      //lcd.print(millis() / 1000);
    case NO_KEY: delay(1); break;
    default: lcd.print(key0); flag=0;        //lcd.setCursor(0, 1);
    }
  }
  
  else if(flag==1) {
    char key1 = keypad.getKey();
    switch(key1){
      case '#' : Serial.println("Numb= "); Serial.println(numb); flag=0; flag0=1; break;
      case '*' : Serial.println(" clearing "); 
                 Serial.println("Numb= "); Serial.println(numb); numb=""; break;
      case NO_KEY: delay(1); break;
      default:  lcd.setCursor(0, 1); lcd.print("Enter Plug No.");  //Serial.println(keyPress); lcd.setCursor(0, 1); 
                lcd.print(key1); numb += key1;  
    }
   }
   
   if (flag0==1) {
    int plg = numb.toInt(); Serial.println(plg);
    lcd.clear(); lcd.print("Mastering Plug-"); lcd.print(plg);
    
    char key2 = keypad.getKey();
    switch(key2){
      case 'B' : lcd.clear(); lcd.print("Master Plug-"); lcd.print(plg); lcd.print("Master 1:");
      case 'C' : lcd.clear(); lcd.print("Master Plug-"); lcd.print(plg); lcd.print("Absolute Value:");
      case 'D' : lcd.clear(); lcd.print("Master Plug-"); lcd.print(plg); lcd.print("SET USL:");
      case '*' : Serial.println(" clearing "); 
                 numb=""; Serial.println("Numb= "); Serial.println(numb); break;
      case NO_KEY: delay(1); break;
      default:  lcd.clear(); lcd.print("Master Plug-"); lcd.print(plg); lcd.print("Wrong Key!");  
    }
    
   }

}
