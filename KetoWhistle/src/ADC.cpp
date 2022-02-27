#include <ADC.hpp>

void ADC_test_setup(Protocentral_ADS1220 *pc_ads1220)
{
    //Serial.begin(9600);

    (*pc_ads1220).begin(ADS1220_CS_PIN, ADS1220_DRDY_PIN);
    (*pc_ads1220).set_data_rate(DR_20SPS);
    (*pc_ads1220).PGA_OFF();
    // pc_ads1220.set_pga_gain(PGA_GAIN_1);
    (*pc_ads1220).select_mux_channels(MUX_AIN0_AVSS);  //Configure for single-ended measurement on AIN0.
    (*pc_ads1220).set_conv_mode_single_shot();          //Set continuous conversion mode

}

void ADC_test_loop(Protocentral_ADS1220 *pc_ads1220)
{
    // if(drdyIntrFlag){
    //     drdyIntrFlag = false;

    //     int32_t adc_data=(*pc_ads1220).Read_Data_Samples();
    //     float Vout = (float)((adc_data*VFSR*1000)/FSR);     //In  mV 

    //     delay(300);

    //     Serial.print("Vout in mV : ");
    //     Serial.print(Vout);
    //     Serial.print("  32bit HEX : ");
    //     Serial.print(adc_data,HEX);
    // }

  int32_t adc_data = pc_ads1220->Read_SingleShot_SingleEnded_WaitForData(MUX_AIN0_AVSS);
  float Vout = (float)((adc_data*VFSR*1000)/FSR);
  delay(500);

  //Serial.print("Vout in mV : ");
  //Serial.print(Vout);
  //Serial.print("  32bit HEX : ");
  //Serial.println(adc_data,HEX);
  
}

float measure_acetone(Protocentral_ADS1220 *pc_ads1220) {
  int32_t adc_data = pc_ads1220->Read_SingleShot_SingleEnded_WaitForData(MUX_AIN0_AVSS);
  float Vout = (float)((adc_data*VFSR*1000)/FSR);
  return Vout;
}

float convert_voltage_to_acetone(float voltage, float baseline) {
  // ((R_air/R_gas - 1) + 0.1055)/0.3289 = concentration
  // baseline = I_1*(1k), voltage = I_2*(R_gas + 1k)
  float V_b = 5.0;
  float R_air = V_b*1000.0/baseline - 1000.0;
  float R_gas = V_b*1000.0/voltage - 1000.0;
  // I_1 = V_batt/(R_air + 1k), baseline = V_b*1k/(R_air + 1k), R_air = (V_b*1k/baseline - 1k), R_gas = (V_b*1k/baseline - 1k)
  float acetone = ((R_air/R_gas) - 1 + 0.1055)/0.3289;
  return acetone;
}

void toggleTempSensorMode(Protocentral_ADS1220 *pc_ads1220) {
  uint8_t curr_value = pc_ads1220->readRegister((uint8_t) 0x01);
  // Toggle TS bit
  curr_value = curr_value ^ ((uint8_t) 0x02);
  // Write Value
  pc_ads1220->writeRegister((uint8_t) 0x01, curr_value);
}

float measure_temperature(Protocentral_ADS1220 *pc_ads1220) {
  int32_t temp_reading = pc_ads1220->Read_SingleShot_SingleEnded_WaitForData(MUX_AIN0_AVSS);
  temp_reading = (temp_reading >> 10);
  float temp = temp_reading * 0.03125;
  return temp;
}