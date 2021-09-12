#include <OLED.hpp>
#include <ADC.hpp>
// #include <BLE.hpp>
#include <scd30_modbus.h>

// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// // BLE test Service
// BLEService testService("FFE0");

// // BLE test Characteristic
// BLECharacteristic testChar("FFE1",  // standard 16-bit characteristic UUID
//     BLERead | BLENotify, 20); // remote clients will be able to get notifications if this characteristic changes
//                               //20 is the maximum size of the characteristic buffer in bytes


void setup()
{
  Serial.begin(9600);
  ADC_Init();
  Serial.println("ADC configuration is complete.");
}

void loop()
{
  ADC_loop();
  Serial.println("Loop iteration complete.");
}
