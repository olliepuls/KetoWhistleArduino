#include <Wire.h>
#include <SPI.h>

// ADC Test Code
#define SCLK 13
#define MISO 12
#define MOSI 11
#define DRDY 7
#define CS 6

// Commands from ADS1220 data sheet
// https://www.ti.com/lit/ds/symlink/ads1220.pdf?ts=1629642933337

#define RESET 0b00000110
#define START 0b00001000
#define READ_DATA 0b00010000

#define V_FULL_SCALE = 2.048

int ADC_Reading = 0;

void reset_ADC() {
  digitalWrite(CS, LOW);
  delayMicroseconds(1);
  SPI.transfer(RESET);
  delayMicroseconds(1);
  delay(50);
  digitalWrite(CS, HIGH);
  
}

void setup_ADC() {
  // Mode = Normal, Smaple Rate = 20SPS, Single Shot Mode
  char reg_1 = 0b00000000;
  char write_command = 0b01000100;
  digitalWrite(CS, LOW);
  delayMicroseconds(1);
  SPI.transfer(write_command);
  SPI.transfer(reg_1);
  delayMicroseconds(1);
  digitalWrite(CS, HIGH);
  delay(50);
}

void trigger_ADC_conversion() {
  digitalWrite(CS, LOW);
  delayMicroseconds(1);
  SPI.transfer(START);
  delayMicroseconds(1);
  digitalWrite(CS, HIGH);
}

// Reads value from ADC into global variable ADC_Reading
// Can be used as an interrupt ISR or a callable function.
void read_ADC_Value() {
  digitalWrite(CS, LOW);
  delayMicroseconds(1);
  // Send Read Data command
  SPI.transfer(READ_DATA);

  // Loop through receiving and bit shift bytes to fill an integer variable
  ADC_Reading = 0;
  int tempByte = 0;
  
  // Receive 3 bytes
  for (int i = 0; i < 3; i++) {
    
    tempByte = SPI.transfer(0x00);
    ADC_Reading = ADC_Reading | tempByte;
    ADC_Reading = ADC_Reading << 8;
  }

  // NOTE: Reading is stored in the 3 most significant bytes of ADC reading in order to preserve sign!
  delayMicroseconds(1);
  digitalWrite(CS, HIGH);
}

double convert_ADC_to_Voltage(int reading) {
  //Negate the 8 bit shift (Now an accurate representation of number of increments from -V_FS to +V_FS)
  reading = reading / 256;
  
  //Multiply Reading By resolution 
  double voltage = (2.048 / 8388608.0)*reading;
  
  return voltage;
}

void setup_ADC() {

  Serial.begin(9600);
  // Configure Board Interrupt (Optional)
  attachInterrupt(digitalPinToInterrupt(7), read_ADC_Value, FALLING);

  // SPI Settings - 10MHz clock is far within ADC timing tolerance.
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE1));

  // Configure pin settings
  pinMode(DRDY, INPUT);
  pinMode(CS, OUTPUT);

  // Reset ADC
  reset_ADC();

  // Configure Modes
  setup_ADC();
}

void loop_ADC() {
  trigger_ADC_conversion();

  delay(100);
  read_ADC_Value();
  double volt = convert_ADC_to_Voltage(ADC_Reading);
  Serial.println(volt);
  delay(1000);
}