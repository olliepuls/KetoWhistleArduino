#include <SparkFun_SCD30_Arduino_Library.h>
#include <ArduinoBLE.h>
#include <Wire.h>

// EEPROM I2C Address
#define EEPROM_I2C_ADDRESS 0x50

#define acetonePin A0
#define BUTTON 7
#define TempSensor A1

SCD30 airSensor;


unsigned long acetoneRead;
const unsigned long heaterTimeOut = 5*60*1000;
unsigned long lastMessage;
unsigned long starttime;
unsigned long endtime;
unsigned long breath_end;

const float Iset = 180.87;//microAmps
const float Rsafe = 25380.0;//Ohms
String string;
uint16_t Acetonemaximum=0;
uint16_t Acetoneminimum=1023;

uint16_t temp_read = 0;
// Integer to hold number of addresses to fill
int address1 = 0;
int maxaddress = 20;


UART mySerial(digitalPinToPinName(3), digitalPinToPinName(4), NC, NC);  // create a hardware serial port named mySerial with RX: pin 4 and TX: pin 3

BLEService AcetoneService("1101");
BLEUnsignedCharCharacteristic AcetoneLevelChar("2101", BLERead | BLENotify);

BLEService StoreService("2101");
BLEUnsignedCharCharacteristic StoreChar("3101", BLERead | BLENotify);

String received = ""; //holds the value of any returned data from the bluetooth module

const int RED_PIN = 14; //The external LED pin
const int YELLOW_PIN = 15; //The external LED pin
const int GREEN_PIN = 16; //The external LED pin

bool heating = false;

void(* reset) (void) = 0;

int val = 0;
int old_val = 0;

int state = 0; // 0 is off, 1 is on

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mySerial.begin(115200);
  Wire.begin();

  uint16_t Acetonemaximum=0;
  uint16_t Acetoneminimum=1023;

  mySerial.println("HEATER OFF");
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);

  pinMode(BUTTON,INPUT);

  airSensor.begin();  
  
  if (!BLE.begin()) 
  {
  Serial.println("starting BLE failed!");
  while (1);
  }

  for (int address = 0; address <= maxaddress; address++){
    // Read value from EEPROM
    temp_read = readEEPROM(address, EEPROM_I2C_ADDRESS);
  }

  BLE.setLocalName("Acetonedetector");
  BLE.setAdvertisedService(AcetoneService);
  AcetoneService.addCharacteristic(AcetoneLevelChar);
  BLE.addService(AcetoneService);
  
  BLE.setLocalName("StoreReadings");
  BLE.setAdvertisedService(StoreService);
  StoreService.addCharacteristic(StoreChar);
  BLE.addService(StoreService);

  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // put your main code here, to run repeatedly:

val = digitalRead(BUTTON);

if ((val == HIGH)&&(old_val == LOW)){
  state = 1 - state;
  delay(10);
  }

old_val = val;



BLEDevice central = BLE.central();

if ((central)&&(state == 1))
{
Serial.print("Connected to central: ");
Serial.println(central.address());


while (central.connected()) {
    
    for (int address = 0; address <= maxaddress; address++){
    // Read value from EEPROM
    temp_read = readEEPROM(address, EEPROM_I2C_ADDRESS);
    StoreChar.writeValue(temp_read);
    delay(10);  
  }

float CurrentTemp = analogRead(TempSensor);
    starttime = millis();
    endtime = starttime;
  while ((endtime - starttime) <=5000){ //every 5 second to check the acetone sensor temp
    if (CurrentTemp < 340) { //determine the temperature of acetone sensor
      heating = true;
      digitalWrite(RED_PIN, HIGH);
      delay(500);
      digitalWrite(RED_PIN, LOW);
      delay(500);      
    } else if (CurrentTemp >= 355) { //currently toggles the LED and enables Heating Regulator
      heating = false;
      digitalWrite(RED_PIN, LOW);
    } else{
      Serial.flush();
    }
  }
    lastMessage = millis();

  if(heating && (millis()-lastMessage>heaterTimeOut)){
    heating = false;
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, HIGH);
    delay(500);
    digitalWrite(GREEN_PIN, LOW);
    delay(500);      
  }

  if (airSensor.dataAvailable()){
    sendData("Humi", uint16_t(airSensor.getHumidity()+0.5));
    delay(10);
    sendData("Temp", uint16_t(airSensor.getTemperature()+0.5));
    delay(10);
    sendData("CO2", airSensor.getCO2());
    delay(10);
  }


    if(millis()-acetoneRead>200){
    float V = analogRead(acetonePin)*5.0/1024.0;
    uint16_t AcetoneLevel = ((V*Rsafe)/((Iset*Rsafe)/1000000.0-V));
    Serial.print("Acetone Level % is now: ");
    Serial.println(AcetoneLevel); 
    acetoneRead = millis();

    string += ":";
    string += AcetoneLevel;
    string += ":";
    string += (unsigned long)(millis() / 100);
    char tmpl[string.length() + 1];
    string.toCharArray(tmpl, string.length() + 1);

    AcetoneLevelChar.writeValue(AcetoneLevel);
    delay(10);

    if (AcetoneLevel<Acetoneminimum){  // determine maximum value
        Acetoneminimum = AcetoneLevel;
      }
    if (AcetoneLevel>Acetonemaximum){
        Acetonemaximum = AcetoneLevel;
      }

    }
    
    breath_end = millis();
    if (millis()-breath_end > 5000){
    if (Acetonemaximum>80){
    digitalWrite(RED_PIN, HIGH);
    }else if (50<Acetonemaximum<80){
    digitalWrite(YELLOW_PIN, HIGH);     
    }else if (0<Acetonemaximum<50){
    digitalWrite(GREEN_PIN, HIGH);     
    }
    if (address1 < maxaddress){
      writeEEPROM(address1, Acetonemaximum, EEPROM_I2C_ADDRESS);
      address1 += 1;
      }
    
    }

}
}

delay(20000);

Serial.print("Disconnected from central: ");
Serial.println(central.address());
digitalWrite(RED_PIN, LOW);
digitalWrite(YELLOW_PIN, LOW);
digitalWrite(GREEN_PIN, LOW);
digitalWrite(BUTTON, LOW);
}

void sendData(String title, uint16_t data) {
  title += ":";
  title += data;
  title += ":";
  title += (unsigned long)(millis() / 100);
  char tmp[title.length() + 1];
  title.toCharArray(tmp, title.length() + 1);
  Serial.write(tmp);
}

// Function to write to EEPROOM
void writeEEPROM(int address, byte Acetonemaximum, int i2c_address)
{
  // Begin transmission to I2C EEPROM
  Wire.beginTransmission(i2c_address);
 
  // Send memory address as two 8-bit bytes
  Wire.write((int)(address >> 8));   // MSB
  Wire.write((int)(address & 0xFF)); // LSB
 
  // Send data to be stored
  Wire.write(Acetonemaximum);
 
  // End the transmission
  Wire.endTransmission();
 
  // Add 5ms delay for EEPROM
  delay(5);
}

// Function to read from EEPROM
byte readEEPROM(int address, int i2c_address)
{
  // Define byte for received data
  byte rcvData = 0xFF;
 
  // Begin transmission to I2C EEPROM
  Wire.beginTransmission(i2c_address);
 
  // Send memory address as two 8-bit bytes
  Wire.write((int)(address >> 8));   // MSB
  Wire.write((int)(address & 0xFF)); // LSB
 
  // End the transmission
  Wire.endTransmission();
 
  // Request one byte of data at current memory address
  Wire.requestFrom(i2c_address, 1);
 
  // Read the data and assign to variable
  rcvData =  Wire.read();
 
  // Return the data as function output
  return rcvData;
}
