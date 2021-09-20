#include <OLED.hpp>
#include <ADC.hpp>
// #include <BLE.hpp>
#include <CO2.hpp>
#include <Misc.hpp>

Protocentral_ADS1220 pc_ads1220;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
SCD30_Modbus scd30;
float current_co2 =0;

void setup()
{
  OLED_setup(&display);
  scd30_test_setup(&scd30);
  ADC_setup(&pc_ads1220);
  configure_button_interrupt();

  draw_anu(&display, 2000);
  draw_logo(&display, 2000);
}

void loop()
{
  // INITIAL STAGE - Waiting for Button Press to commence heating cycle. Take baseline CO2 level measurement.
  float baseline_co2 = measure_CO2(&scd30);

  while (!button_interrupt_flag) {
    draw_anu(&display, 1500);
    draw_logo(&display, 1500);
    button_prompt(&display, 2000);
  }
  button_interrupt_flag = false; // Reset button interrupt.

  // HEATING STAGE - Display Temperature 
  float temp = 0.0;
  while (temp < 300) {
    // TODO: Calibrate temp sensor to return actual voltages.
    // temp = analogRead(TEMP_PIN);
    temp += 50.0;
    display_temperature(&display, 800, temp);
  }

  // BREATH STAGE - Prompt user to breathe into tube. Monitor CO2 levels for increase.
  bool breath_detected = false;
  float acetone_level;
  while (!breath_detected) {
    breath_prompt(&display);
    float current_co2 = 0.0;
    
    for (int i = 0; i < 5; i++) {
      current_co2 = measure_CO2(&scd30);
      //Serial.print(baseline_co2);
      //Serial.print("  ");
      //Serial.println(current_co2);
      if (current_co2 > baseline_co2 + 500) {break;}
      delay(2000);
    }

    if (current_co2 > baseline_co2 + 500) {
      acetone_level = measure_acetone(&pc_ads1220);
      breath_detected = true;
    } else {
      breath_abort_prompt(&display, 1000);
      baseline_co2 = min(current_co2, baseline_co2);
    }
    acetone_level = measure_acetone(&pc_ads1220);
  }

  // RESULTS DISPLAY - Display acetone measurement results.
  display_acetone_results(&display, 2000, acetone_level);
}
