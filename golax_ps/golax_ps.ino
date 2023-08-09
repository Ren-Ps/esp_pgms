#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

//INPUT INTTERUPUT GPIO
#define IGD0 D2
#define IGD1 D3
#define IGD2 D4
#define IGD3 D5

//OUTPUT GPIO
#define SP0 D6
#define SP1 D7
#define SP2 D8
#define SP3 D10

LiquidCrystal_PCF8574 lcd(0x27); // set the LCD address to 0x27 for a 16 chars & 2 line display
int show, error, rpm;
int hold[4];
const int thshld=30;

void int1() {
   digitalWrite(SP0, HIGH); hold[0]=1;
}

void int1() {
   digitalWrite(SP0, HIGH); hold[0]=1;
}

void int2() {
   digitalWrite(SP1, HIGH); hold[1]=1;
}

void int3() {
   digitalWrite(SP2, HIGH); hold[2]=1;
}

void int4() {
   digitalWrite(SP3, HIGH); hold[3]=1;
}

void setup() {
  pinMode(IGD0, INPUT);  pinMode(IGD1, INPUT); pinMode(IGD2, INPUT); pinMode(IGD3, INPUT);
  pinMode(SP0, OUTPUT); pinMode(SP1, OUTPUT); pinMode(SP2, OUTPUT); pinMode(SP3, OUTPUT);
  Serial.begin(115200);
  
  attachInterrupt(IGD0, int1, RISING);
  attachInterrupt(IGD1, int2, RISING);
  attachInterrupt(IGD2, int3, RISING);
  attachInterrupt(IGD3, int4, RISING);
  
  Wire.begin();
  Wire.beginTransmission(0x27);
  error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);
  if (error == 0) {
    Serial.println(": LCD found.");
  } else {
    Serial.println(": LCD not found.");
  } // if
  lcd.begin(16, 2); // initialize the lcd
  show = 0;

}

void loop() {
  
  if (show == 0) {
    lcd.setBacklight(255);
    lcd.home(); lcd.clear();
    lcd.print("Hello LCD");
    delay(1000);

    lcd.setBacklight(0);
    delay(400);
    lcd.setBacklight(255);

  } else if (show == 1) {
    lcd.clear();
    lcd.print("Cursor On");
    lcd.cursor();

  } else if (show == 2) {
    lcd.clear();
    lcd.print("Cursor Blink");
    lcd.blink();

  } else if (show == 3) {
    lcd.clear();
    lcd.print("Cursor OFF");
    lcd.noBlink();
    lcd.noCursor();

  } else if (show == 4) {
    lcd.clear();
    lcd.print("Display Off");
    lcd.noDisplay();

  } else if (show == 5) {
    lcd.clear();
    lcd.print("Display On");
    lcd.display();

  } else if (show == 7) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*** first line.");
    lcd.setCursor(0, 1);
    lcd.print("*** second line.");

  } else if (show == 8) {
    lcd.scrollDisplayLeft();
  } else if (show == 9) {
    lcd.scrollDisplayLeft();
  } else if (show == 10) {
    lcd.scrollDisplayLeft();
  } else if (show == 11) {
    lcd.scrollDisplayRight();
  } // if
  delay(2000);
  show = (show + 1) % 12;

  if(rpm >= thshld) {
   digitalWrite(D16, HIGH);
   lcd.print("RPM is Greater Than threashould");
   lcd.print(rmp);
  }
  else {
    digitalWrite(D15, LOW);
  }
  
}
