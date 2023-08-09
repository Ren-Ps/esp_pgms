// the MAX7219 address map (datasheet table 2)
#define MAX7219_DECODE_REG      (0x09)
#define MAX7219_INTENSITY_REG   (0x0A)
#define MAX7219_SCANLIMIT_REG   (0x0B)
#define MAX7219_SHUTDOWN_REG    (0X0C)
#define MAX7219_DISPLAYTEST_REG (0x0F)
#define MAX7219_DIGIT_REG(pos)  ((pos) + 1)

// shutdown mode (datasheet table 3)
#define MAX7219_OFF             (0x0)
#define MAX7219_ON              (0x1)

// pin 13 of MAX7219 (CLK)
const int clock_pin = 4;
// pin 12 of MAX7219 (LOAD)
const int data_latch_pin = 3;
// pin 1 of MAX7219 (DIN)
const int data_input_pin = 2;

// digit pattern for a 7-segment display. datasheet table 5
const byte digit_pattern[16] =
{
  B01111110,  // 0
  B00110000,  // 1
  B01101101,  // 2
  B01111001,  // 3
  B00110011,  // 4
  B01011011,  // 5
  B01011111,  // 6
  B01110000,  // 7
  B01111111,  // 8
  B01111011,  // 9
  B01110111,  // A
  B00011111,  // b
  B01001110,  // C
  B00111101,  // d
  B01001111,  // E
  B01000111   // F
};

#define DP_FLAG       (B10000000)
#define NUM_OF_DIGITS (8)

unsigned int counter = 0;
unsigned int digit_base = 16;

// update the register value of MAX7219
void set_register(byte address, byte value)  
{
  digitalWrite(data_latch_pin, LOW);
  shiftOut(data_input_pin, clock_pin, MSBFIRST, address);
  shiftOut(data_input_pin, clock_pin, MSBFIRST, value);
  digitalWrite(data_latch_pin, HIGH);
}

void init_max7219()
{
  // disable test mode. datasheet table 10
  set_register(MAX7219_DISPLAYTEST_REG, MAX7219_OFF);
  // set medium intensity. datasheet table 7
  set_register(MAX7219_INTENSITY_REG, 0x8);
  // turn off display. datasheet table 3
  set_register(MAX7219_SHUTDOWN_REG, MAX7219_OFF);
  // drive 8 digits. datasheet table 8
  set_register(MAX7219_SCANLIMIT_REG, 7);
  // no decode mode for all positions. datasheet table 4
  set_register(MAX7219_DECODE_REG, B00000000);
}

void setup()  
{
  // init pin states
  pinMode(clock_pin, OUTPUT);
  pinMode(data_latch_pin, OUTPUT);    
  pinMode(data_input_pin, OUTPUT);

  // init MAX2719 states
 // init_max7219();
}

void loop()  
{
  set_register(MAX7219_DISPLAYTEST_REG, MAX7219_ON);
  delay(50);
}
