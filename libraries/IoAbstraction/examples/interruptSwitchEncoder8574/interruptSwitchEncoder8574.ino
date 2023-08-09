/*
 This sketch is an example of using a rotary encoder along with two switches and an LED over an i2c 8574 IO expander. 

 One switch is the encoders push switch, configured as non-repeating. The other switch is repeating and will toggle the LED.
 In this example the switches library is set up in interrupt mode, which means unless something is pressed down, there is no
 polling.

 Switch input is designed to work with the task manager class which makes scheduling tasks trivial.

 Circuit / detail: https://www.thecoderscorner.com/products/arduino-libraries/io-abstraction/rotary-encoder-switches-interrupt-pcf8574/

*/

#include<IoAbstraction.h>
#include<IoAbstractionWire.h>

// The pin onto which we connected the rotary encoders switch
const int spinwheelClickPin = 4;

// The pin onto which we connected the repeat button switch
const int repeatButtonPin = 0;

// the led pin on the IO ioDevice
const int ledPin = 1;

// The two pins where we connected the A and B pins of the encoder. I recomend you dont change these
// as the pin must support interrupts.
const int encoderAPin = 2;
const int encoderBPin = 3;

// the maximum (0 based) value that we want the encoder to represent.
const int maximumEncoderValue = 128;

// used when we toggle the LED state further down.
bool currentLedState;

//
// When the spinwheel is clicked, this function will be run as we registered it as a callback
//
void onSpinwheelClicked(uint8_t pin, bool heldDown) {
  Serial.print("Encoder button pressed ");
  Serial.println(heldDown ? "Held" : "Pressed");
}

//
// When the repeat button is pressed, this function will be repeatedly called. It's also a callback
//
void onRepeatButtonClicked(uint8_t pin, bool heldDown) {
  Serial.print("Repeat button ");
  Serial.println(heldDown ? "Held" : "Pressed");

  // Flip the state of the led by reading, fliping then writing it's value
  // notice the use of the 'S' version of the ioDevice functions, these are
  // for single operations, where it includes a sync with the io device
  currentLedState = !currentLedState;
  ioDeviceDigitalWriteS(switches.getIoAbstraction(), ledPin, currentLedState);
}

//
// Each time the encoder value changes, this function runs, as we registered it as a callback
//
void onEncoderChange(int newValue) {
  Serial.print("Encoder change ");
  Serial.println(newValue);
}

void setup() {

  // Before doing anything else, we must initialise the wire and serial libraries, as we are using both.
  Serial.begin(115200);
  Wire.begin();

  // First we set up the switches library, giving it the task manager and tell it where the pins are located
  // We could also of chosen IO through an i2c device that supports interrupts.
  // If you want to use PULL UP instead of PULL DOWN logic, uncomment the additional parameter below
  switches.initialiseInterrupt(ioFrom8574(0x20, 2), true);

  ioDevicePinMode(switches.getIoAbstraction(), ledPin, OUTPUT);

  // now we add the switches, we dont want the spinwheel button to repeat, so leave off the last parameter
  // which is the repeat interval (millis / 20 basically) Repeat button does repeat as we can see.
  switches.addSwitch(spinwheelClickPin, onSpinwheelClicked);
  switches.addSwitch(repeatButtonPin, onRepeatButtonClicked, 25);

  // now we set up the rotary encoder, first we give the A pin and the B pin.
  // we give the encoder a max value of 128, always minumum of 0.
  setupRotaryEncoderWithInterrupt(encoderAPin, encoderBPin, onEncoderChange);
  switches.changeEncoderPrecision(maximumEncoderValue, 100);
}

void loop() {
  taskManager.runLoop();  
}
