#define GPIO_SEL_pin 10                         // D14 = MCP23S08 GPIO SPI select pin
#define SCLK 13                                 // D18 SPI SCLK 
#define MISO 12                                 // D19 SPI MISO
#define MOSI 11                                 // D23 SPI MOSI

#include <SPI.h>
uint8_t keyboard;
static const int spiClk = 100000; // 100 KHz

//uninitalised pointers to SPI objects
SPIClass * mcp23s08 = NULL;

uint8_t mcp23s08Read ( void ) {
  digitalWrite(GPIO_SEL_pin, LOW);         //pull SS slow to prep other end for transfer
  uint8_t dev_adr  = 0x41;                        // MCP23S08 address 0; R/W = read
  uint8_t reg_adr  = 0x09;                        // MCP23S08 GPIO register
  uint8_t reg_data;                               // GPIO register
  mcp23s08->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  mcp23s08->transfer(dev_adr);
  mcp23s08->transfer(reg_adr);
  reg_data = mcp23s08->transfer(0x00);
  mcp23s08->endTransaction();
  digitalWrite(GPIO_SEL_pin, HIGH);               //pull ss high to signify end of data transfer
  return reg_data;
}


// ================================== routines =================================
void mcp23s08SetUp( void ) {
  uint8_t dev_adr  = 0x40;                          // MCP23S08 address 0; R/W = write
  uint8_t reg_adr  = 0x00;                          // MCP23S08 IODIR register
  uint8_t reg_data = 0xFF;                          // pin configured as input
  digitalWrite(GPIO_SEL_pin, LOW);                 //pull SS slow to prep other end for transfer
  // ============================= MCP3S08 pin as input ==============================
  mcp23s08->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  mcp23s08->transfer(dev_adr);
  mcp23s08->transfer(reg_adr);
  mcp23s08->transfer(reg_data);
  mcp23s08->endTransaction();
  reg_adr  = 0x01;                                 // MCP23S08 IPOL register
  reg_data = 0xFF;                                 // invert input polarity
  // ============================= MCP3S08 invert input ==============================
  mcp23s08->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  mcp23s08->transfer(dev_adr);
  mcp23s08->transfer(reg_adr);
  mcp23s08->transfer(reg_data);
  mcp23s08->endTransaction();
  reg_adr  = 0x06;                                 // MCP23S08 GPPU pull-up register
  reg_data = 0xFF;                                 // set pull-up register
  // ============================= MCP3S08 pullup resistor ==============================
  mcp23s08->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  mcp23s08->transfer(dev_adr);
  mcp23s08->transfer(reg_adr);
  mcp23s08->transfer(reg_data);
  mcp23s08->endTransaction();
  digitalWrite(GPIO_SEL_pin, HIGH);                //pull ss high to signify end of data transfer
}


void setup() {
  Serial.begin(115200);
  pinMode(GPIO_SEL_pin, OUTPUT);                     //MCP23S08 SPI select pin
  digitalWrite(GPIO_SEL_pin, HIGH);                  //pull ss high to disable data transfer
  mcp23s08 = new SPIClass();
  mcp23s08->begin(SCLK, MISO, MOSI, GPIO_SEL_pin);   //SCLK, MISO, MOSI, SS
  mcp23s08SetUp();
  delay(1);
}

void loop() {
  keyboard = mcp23s08Read();
  Serial.println (keyboard);
  delay(100);
}
