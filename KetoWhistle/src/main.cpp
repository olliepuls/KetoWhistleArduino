#include <OLED.hpp>
#include <ADC.hpp>
// #include <BLE.hpp>
#include <CO2.hpp>

Protocentral_ADS1220 pc_ads1220;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// // BLE test Service
// BLEService testService("FFE0");

// // BLE test Characteristic
// BLECharacteristic testChar("FFE1",  // standard 16-bit characteristic UUID
//     BLERead | BLENotify, 20); // remote clients will be able to get notifications if this characteristic changes
//                               //20 is the maximum size of the characteristic buffer in bytes

SCD30_Modbus scd30;

void setup()
{
  // Serial.begin(9600);
  OLED_setup(&display);
  // scd30_test_setup(&scd30);
  // Serial.println("ADC configuration is complete.");
  ADC_setup(&pc_ads1220);
  draw_anu(&display, 2000);
}

void loop()
{
  // disp_loop(&display);
  // Serial.println("Loop iteration complete.");
  // scd30_test_loop(&scd30);
  

  float ave_voltage = 0.0;
  String volts;
  for(int i = 0; i < 5; i++) {
    int32_t adc_data = pc_ads1220.Read_SingleShot_SingleEnded_WaitForData(MUX_AIN0_AVSS);
    float Vout = (float)((adc_data*VFSR*1000)/FSR);
    ave_voltage += Vout;
    volts = String(Vout, 3);

    Devicetext(&display, "Voltage", 20, 0, 2, false);
    Devicetext(&display, "Vout:   ", 10, 25, 1, false);
    Devicetext(&display, volts, 45, 25, 1, false);
    Devicetext(&display, "mV", 100, 25, 1, false);
    display.display();

    delay(500);
    display.clearDisplay();
  }

  ave_voltage = ave_voltage / 5;
  volts = String(ave_voltage, 3);
  Devicetext(&display, "Average", 20, 0, 2, false);
  Devicetext(&display, "Vave:   ", 10, 25, 1, false);
  Devicetext(&display, volts, 45, 25, 1, false);
  Devicetext(&display, "mV", 100, 25, 1, false);
  display.display();
  delay(1000);
  display.clearDisplay();
  
}
