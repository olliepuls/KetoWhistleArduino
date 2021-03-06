#include <OLED.hpp>
#include <ADC.hpp>
#include <BLE.hpp>
#include <CO2.hpp>
#include <Misc.hpp>

// Pin Definitions
#define ENAB_1 9

// CO2 Detection Threshold

Protocentral_ADS1220 pc_ads1220;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
SCD30_Modbus scd30;
NanoBLEFlashPrefs myFlashPrefs;

// BLE test Service
BLEService testService("FFE0");

// BLE test Characteristic
BLEUnsignedIntCharacteristic acetoneCharacteristic("2A6C", BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes
BLEUnsignedIntCharacteristic deviceTempCharacteristic("2A6D", BLERead | BLENotify);

BLEDevice central;

// CO2 Global Variable
float current_co2 = 10.0;
float co2_threshold = 200.0F;



void ketoWhistle_setup() {
  OLED_setup(&display);
  scd30_test_setup(&scd30);
  ADC_test_setup(&pc_ads1220);
  BLE_setup(&testService, &acetoneCharacteristic, &deviceTempCharacteristic);

  configure_button_interrupt();


  // Set Enable Pin for acetone sensor to OFF
  digitalWrite(ENAB_1, LOW);
}

void ketoWhistle_loop() {
  // INITIAL STAGE - Waiting for Button Press to commence heating cycle. Take baseline CO2 level measurement.
  
  button_interrupt_flag = false; // Reset button interrupt.
  
  draw_anu(&display);
  delay(2000);
  draw_logo(&display);
  delay(2000);

  // Connect to phone - Maybe add a prompt/button press to do this.
  central = BLE.central();
  
  while (!button_interrupt_flag) {
    button_prompt(&display);
    delay_on_flag(2000, 100, button_interrupt_flag);
    if (button_interrupt_flag) {break;}
  }

  // button_interrupt_flag = false; // Reset button interrupt.

  // Check Tube Temperature
  toggleTempSensorMode(&pc_ads1220);
  float temp = measure_temperature(&pc_ads1220);
  toggleTempSensorMode(&pc_ads1220);
  
  if (temp > 40.0) {
    Devicetext(&display, "Too hot!", 10, 5, 1, false);
    Devicetext(&display, "Wait for cooldown...", 10, 15, 1, false);
    delay(10000);
  } else {
  
    // HEATING STAGE - Display Temperature 
    // Turn on Acetone heater & Sensor Circuit
    digitalWrite(ENAB_1, HIGH);
    float temp = 0.0;
    while (temp < 400) {
      // TODO: Calibrate temp sensor to return actual voltages.
      // temp = analogRead(TEMP_PIN);
      temp += 50.0;
      display_temperature(&display, 800, temp);
    }

    // BREATH STAGE - Prompt user to breathe into tube. Monitor CO2 levels for increase.
    bool breath_detected = false;
    button_interrupt_flag = false; // Reset button interrupt.
    
    float baseline_voltage = measure_acetone(&pc_ads1220);
    float acetone_level = 0.0;

    float baseline_co2 = measure_CO2(&scd30);

    while (!(breath_detected || button_interrupt_flag)) {
      breath_prompt(&display);
      
      for (int i = 0; i < 5; i++) {
        
        //Serial.print(baseline_co2);
        //Serial.print("  ");
        //Serial.println(current_co2);
        // display_acetone_results(&display, 2000, co2_threshold);
        current_co2 = measure_CO2(&scd30);
        // display_acetone_results(&display, 2000, current_co2 - baseline_co2);
        if ((current_co2 - baseline_co2) > co2_threshold) {
          breath_detected = 1;
          break;
        }
        
        if (button_interrupt_flag) {
          break;
        }
        delay(2000);
      }

      if (breath_detected || button_interrupt_flag) {
        acetone_level = measure_acetone(&pc_ads1220);
        // acetone_level = 0.51;
      } else {
        breath_abort_prompt(&display, 2000);
        baseline_co2 = min(current_co2, baseline_co2);
      }
    }
    delay(5000);
    tone(A2, 1000, 2000);

    digitalWrite(ENAB_1, LOW);
    // Send results to Bluetooth if connected.
    
    if (central && central.connected()) {
      acetoneCharacteristic.writeValue((uint32_t) acetone_level);
    }
    
    acetone_level = convert_voltage_to_acetone(acetone_level, baseline_voltage);
    // RESULTS DISPLAY - Display acetone measurement results.
    display_acetone_results(&display, 15000, acetone_level);
  }
}

void setup()
{
  BLE_setup(&testService, &acetoneCharacteristic, &deviceTempCharacteristic);
  // ketoWhistle_setup();
}

void loop()
{
  // ketoWhistle_loop();
  loop_BLE(&testService, &acetoneCharacteristic, &deviceTempCharacteristic);
}

