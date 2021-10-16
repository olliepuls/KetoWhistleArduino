#include <OLED.hpp>
#include <ADC.hpp>
#include <BLE.hpp>
#include <CO2.hpp>
#include <Misc.hpp>

// Pin Definitions
#define ENAB_1 9

// CO2 Detection Threshold
#define CO2_THRESHOLD 200

Protocentral_ADS1220 pc_ads1220;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
SCD30_Modbus scd30;
NanoBLEFlashPrefs myFlashPrefs;

// BLE test Service
BLEService testService("FFE0");

// BLE test Characteristic
BLEUnsignedIntCharacteristic acetoneCharacteristic("2A6C", BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes

// CO2 Global Variable
float current_co2 = 0.0;



void ketoWhistle_setup() {
  OLED_setup(&display);
  scd30_test_setup(&scd30);
  ADC_test_setup(&pc_ads1220);
  BLE_setup(&testService, &acetoneCharacteristic);

  configure_button_interrupt();


  // Set Enable Pin for acetone sensor to OFF
  digitalWrite(ENAB_1, LOW);
}

void ketoWhistle_loop() {
  // INITIAL STAGE - Waiting for Button Press to commence heating cycle. Take baseline CO2 level measurement.
  float baseline_co2 = measure_CO2(&scd30);
  button_interrupt_flag = false; // Reset button interrupt.
  while(1) {
    current_co2 = measure_CO2(&scd30);
    display_acetone_results(&display, 2000, current_co2);
  }
  
  while (!button_interrupt_flag) {
    draw_anu(&display);
    delay_on_flag(2000, 100, button_interrupt_flag);
    if (button_interrupt_flag) {break;}
    draw_logo(&display);
    delay_on_flag(2000, 100, button_interrupt_flag);
    if (button_interrupt_flag) {break;}
    button_prompt(&display);
    delay_on_flag(2000, 100, button_interrupt_flag);
    if (button_interrupt_flag) {break;}
  }

  // button_interrupt_flag = false; // Reset button interrupt.
  
  // HEATING STAGE - Display Temperature 
  // Turn on Acetone heater & Sensor Circuit
  digitalWrite(ENAB_1, HIGH);
  float temp = 0.0;
  while (temp < 300) {
    // TODO: Calibrate temp sensor to return actual voltages.
    // temp = analogRead(TEMP_PIN);
    temp += 50.0;
    display_temperature(&display, 800, temp);
  }

  // BREATH STAGE - Prompt user to breathe into tube. Monitor CO2 levels for increase.
  bool breath_detected = false;
  button_interrupt_flag = false; // Reset button interrupt.
  float acetone_level = 12.2;
  while (!breath_detected) {
    breath_prompt(&display);
    
    for (int i = 0; i < 5; i++) {
      current_co2 = measure_CO2(&scd30);
      display_acetone_results(&display, 2000, current_co2);
      //Serial.print(baseline_co2);
      //Serial.print("  ");
      //Serial.println(current_co2);
      if (current_co2 > baseline_co2 + CO2_THRESHOLD) {break;}
      // delay(2000);
    }

    if (current_co2 > baseline_co2 + CO2_THRESHOLD) {
      acetone_level = measure_acetone(&pc_ads1220);
      breath_detected = true;
    } else {
      breath_abort_prompt(&display, 1000);
      baseline_co2 = min(current_co2, baseline_co2);
    }
    // acetone_level = measure_acetone(&pc_ads1220);
  }


  // while (!button_interrupt_flag) {
  //   delay(1);
  // }
  tone(A2, 1000, 1000);

  digitalWrite(ENAB_1, LOW);
  // Send results to Bluetooth if connected.
  
  if (BLE.connected()) {
    acetoneCharacteristic.writeValue((uint32_t) acetone_level);
  }
  
  // RESULTS DISPLAY - Display acetone measurement results.
  display_acetone_results(&display, 10000, acetone_level);
}

void setup()
{
  // BLE_setup(&testService, &acetoneCharacteristic);
  ketoWhistle_setup();
}

void loop()
{
  ketoWhistle_loop();
  // loop_BLE(&testService, &acetoneCharacteristic);
}

