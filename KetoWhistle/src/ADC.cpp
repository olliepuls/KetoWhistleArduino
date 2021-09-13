#include "ADC.hpp"

void reset_ADC() {
  digitalWrite(CS_ADC, LOW);
  delayMicroseconds(1);
  SPI.transfer(RESET_ADC);
  delayMicroseconds(1);
  delay(50);
  digitalWrite(CS_ADC, HIGH);
  
}

void init_ADC() {
  // Mode = Normal, Smaple Rate = 20SPS, Single Shot Mode
  char reg_1 = 0b00000000;
  char write_command = 0b01000100;
  digitalWrite(CS_ADC, LOW);
  delayMicroseconds(1);
  SPI.transfer(write_command);
  SPI.transfer(reg_1);
  delayMicroseconds(1);
  digitalWrite(CS_ADC, HIGH);
  delay(50);
}

void trigger_ADC_conversion() {
  digitalWrite(CS_ADC, LOW);
  delayMicroseconds(1);
  SPI.transfer(START_ADC);
  delayMicroseconds(1);
  digitalWrite(CS_ADC, HIGH);
}

void read_ADC_Value(int *ADC_Reading) {
  digitalWrite(CS_ADC, LOW);
  delayMicroseconds(1);
  // Send Read Data command
  SPI.transfer(READ_DATA_ADC);

  // Loop through receiving and bit shift bytes to fill an integer variable
  *ADC_Reading = 0;
  int tempByte = 0;
  
  // Receive 3 bytes
  for (int i = 0; i < 3; i++) {
    
    tempByte = SPI.transfer(0x00);
    *ADC_Reading = *ADC_Reading << 8;
    *ADC_Reading = *ADC_Reading | tempByte;
  }

  // NOTE: Reading is stored in the 3 most significant bytes of ADC reading in order to preserve sign!
  delayMicroseconds(1);
  digitalWrite(CS_ADC, HIGH);
}

double convert_ADC_to_Voltage(int reading) {
  
  //Multiply Reading By resolution 
  double voltage = (2.048 / 8388608.0)*reading;
  
  return voltage;
}

void ADC_setup() {

  // Configure Board Interrupt (Optional)
//   attachInterrupt(digitalPinToInterrupt(7), read_ADC_Value, FALLING);
  // SPI Settings - 10MHz clock is far within ADC timing tolerance.
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE1));

  // Configure pin settings
  pinMode(DRDY_ADC, INPUT);
  pinMode(CS_ADC, OUTPUT);

  // Reset ADC
  reset_ADC();

  // Configure Modes
  init_ADC();
}

void ADC_loop() {
  trigger_ADC_conversion();
  int ADC_Reading;
  delay(100);
  read_ADC_Value(&ADC_Reading);
  double volt = convert_ADC_to_Voltage(ADC_Reading);
  Serial.println(volt);
  delay(1000);
}