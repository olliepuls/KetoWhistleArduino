#include "Protocentral_ADS1220.h"
#include <SPI.h>

#define PGA 1                // Programmable Gain, confirm that the same as set_pga_gain
#define VREF 2.048            // Internal reference of 2.048V
#define VFSR VREF/PGA
#define FSR (((long int)1<<23)-1)

#define ADS1220_CS_PIN    6
#define ADS1220_DRDY_PIN  7

// volatile bool drdyIntrFlag = false;
void drdyInterruptHndlr();
void enableInterruptPin();
void ADC_test_setup(Protocentral_ADS1220 *pc_ads1220);

void ADC_test_loop(Protocentral_ADS1220 *pc_ads1220);
float measure_acetone(Protocentral_ADS1220 *pc_ads1220);