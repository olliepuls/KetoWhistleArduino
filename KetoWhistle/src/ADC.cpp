#include <ADC.hpp>

void ADC_test_setup(Protocentral_ADS1220 *pc_ads1220)
{
    //Serial.begin(9600);

    (*pc_ads1220).begin(ADS1220_CS_PIN, ADS1220_DRDY_PIN);
    (*pc_ads1220).set_data_rate(DR_20SPS);
    (*pc_ads1220).PGA_OFF();
    // pc_ads1220.set_pga_gain(PGA_GAIN_1);
    (*pc_ads1220).select_mux_channels(MUX_AIN0_AVSS);  //Configure for differential measurement between AIN0 and AIN1
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
  // float acetone = convert_voltage_to_acetone(Vout);
  return Vout;
}

// Placeholder Function
float convert_voltage_to_acetone(float voltage) {
  float acetone = voltage;
  return acetone;
}