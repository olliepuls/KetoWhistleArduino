// ADC Test Code
#define SCLK_ADC 13
#define MISO_ADC 12
#define MOSI_ADC 11
#define DRDY_ADC 7
#define CS_ADC 6

// Commands from ADS1220 data sheet
// https://www.ti.com/lit/ds/symlink/ads1220.pdf?ts=1629642933337

#define RESET_ADC 0b00000110
#define START_ADC 0b00001000
#define READ_DATA_ADC 0b00010000

#define V_FULL_SCALE = 2.048

#include <Wire.h>
#include <SPI.h>

void reset_ADC();
void setup_ADC();

void trigger_ADC_conversion();
void read_ADC_Value(int *ADC_Reading);

double convert_ADC_to_Voltage(int reading);

void ADC_Init();

void ADC_loop();