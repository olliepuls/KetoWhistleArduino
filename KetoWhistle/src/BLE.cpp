// #include <BLE.hpp>

// void setup_BLE(BLEService *service, BLECharacteristic *characteristic) {
//   // begin initialization
//   if (!BLE.begin()) {
//   }
  
//   /* Set a local name for the BLE device
//      This name will appear in advertising packets
//      and can be used by remote devices to identify this BLE device
//      The name can be changed but maybe be truncated based on space left in advertisement packet
//   */
//   BLE.setLocalName("testMonitor");
//   BLE.setAdvertisedService(*service); // add the service UUID
//   service->addCharacteristic(*characteristic); // add the test level characteristic
//   BLE.addService(*service); // Add the test service

//   /* Start advertising BLE.  It will start continuously transmitting BLE
//      advertising packets and will be visible to remote BLE central devices
//      until it receives a new connection */

//   // start advertising
//   BLE.advertise();
// }

