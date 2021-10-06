

#include <BLE.hpp>
#ifdef BLE_DEFINED
void BLE_setup(BLEService *service, BLECharacteristic *characteristic) {
  // begin initialization
  if (!BLE.begin()) {
  }
  
  /* Set a local name for the BLE device
     This name will appear in advertising packets
     and can be used by remote devices to identify this BLE device
     The name can be changed but maybe be truncated based on space left in advertisement packet
  */
  BLE.setLocalName("KetoWhistle");
  BLE.setAdvertisedService(*service); // add the service UUID
  service->addCharacteristic(*characteristic); // add the test level characteristic
  BLE.addService(*service); // Add the test service
  BLE.setConnectable(true);

  /* Start advertising BLE.  It will start continuously transmitting BLE
     advertising packets and will be visible to remote BLE central devices
     until it receives a new connection */

  // start advertising
  BLE.advertise();
}

void loop_BLE(BLEService *service, BLECharacteristic *characteristic) {
  // wait for a BLE central (phone)
  BLEDevice central = BLE.central();

  // if a central is connected to the peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's BT address:
    Serial.println(central.address());
    // turn on the LED to indicate the connection:
    digitalWrite(LED_BUILTIN, HIGH);

    // check the test level every 200ms
    // while the central is connected:
    uint32_t data = 0;

    while (central.connected()) {
      // if 200ms have passed, check the test level:
      (*characteristic).writeValue((uint32_t) data);  
      data += 1;
      delay(1000);
    }
    
    // when the central disconnects, turn off the LED:
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}
#endif