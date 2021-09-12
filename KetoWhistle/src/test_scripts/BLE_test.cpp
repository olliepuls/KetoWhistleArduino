// #include <ArduinoBLE.h>

// // BLE test Service
// BLEService testService("FFE0");

// // BLE test Characteristic
// BLECharacteristic testChar("FFE1",  // standard 16-bit characteristic UUID
//     BLERead | BLENotify, 20); // remote clients will be able to get notifications if this characteristic changes
//                               //20 is the maximum size of the characteristic buffer in bytes
// long previousMillis = 0;  // last time the test level was checked, in ms

// void setup_BLE() {
//   Serial.begin(9600);    // initialize serial communication to computer
//   while (!Serial);  //requires an established serial port to run the rest of the code(?)

//   pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in LED pin to indicate when a central is connected

//   // begin initialization
//   if (!BLE.begin()) {
//     Serial.println("starting BLE failed!");

//     while (1);
//   }

//   /* Set a local name for the BLE device
//      This name will appear in advertising packets
//      and can be used by remote devices to identify this BLE device
//      The name can be changed but maybe be truncated based on space left in advertisement packet
//   */
//   BLE.setLocalName("testMonitor");
//   BLE.setAdvertisedService(testService); // add the service UUID
//   testService.addCharacteristic(testChar); // add the test level characteristic
//   BLE.addService(testService); // Add the test service

//   /* Start advertising BLE.  It will start continuously transmitting BLE
//      advertising packets and will be visible to remote BLE central devices
//      until it receives a new connection */

//   // start advertising
//   BLE.advertise();

//   Serial.println("Bluetooth device active, waiting for connections...");
// }

// void loop_BLE() {
//   // wait for a BLE central (phone)
//   BLEDevice central = BLE.central();

//   // if a central is connected to the peripheral:
//   if (central) {
//     Serial.print("Connected to central: ");
//     // print the central's BT address:
//     Serial.println(central.address());
//     // turn on the LED to indicate the connection:
//     digitalWrite(LED_BUILTIN, HIGH);

//     // check the test level every 200ms
//     // while the central is connected:
//     while (central.connected()) {
//       long currentMillis = millis();
//       // if 200ms have passed, check the test level:
//       if (currentMillis - previousMillis >= 200) {
//         previousMillis = currentMillis;
//         String string = ":data:3";
//         char tmp[string.length()+1];
//         string.toCharArray(tmp, string.length()+1);
//         //Serial.print(tmp);
//         testChar.writeValue(tmp);  
//       }
//     }
//     // when the central disconnects, turn off the LED:
//     digitalWrite(LED_BUILTIN, LOW);
//     Serial.print("Disconnected from central: ");
//     Serial.println(central.address());
//   }
// }