#include <OLED.hpp>
#include <ADC.hpp>
// #include <BLE.hpp>
#include <CO2.hpp>

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
  // disp_setup(&display);
  scd30_test_setup(&scd30);
  // Serial.println("ADC configuration is complete.");
}

void loop()
{
  // disp_loop(&display);
  // Serial.println("Loop iteration complete.");
  scd30_test_loop(&scd30);
}
