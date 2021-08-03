#include <EEPROM.h>
 
#define SAMPLE_TIME 2000  //The time between each EEPROM write function call in ms
 
int tempPin = 0;      //the ADC pin
int printPin = 2;     //the print button pin
int erasePin = 4;    //the erase button pin
 
int address = 0;      //EEPROM address counter
 
unsigned long timer;
 
float conv_coeff = 0.0;   //coefficient for converting from 0-1024 to 0-5 range
 
void printTemp();
void clearEEPROM();
void writeTemp();
 
void setup(){
  Serial.begin(115200);     //start the serial connection as always
  conv_coeff = 5.0/1024.0;  //find the coefficient to do the conversion
  timer = millis();         //millis() returns the time since program start in ms
}
 
void loop(){
  if(millis()-timer > SAMPLE_TIME)  //check if it's time to do a temp sensor sample
  {
    writeTemp();
    timer = millis();
  }
 
  if(!digitalRead(printPin))  //check if print button is pressed
  {
    printTemp();
    delay(500);
  }
 
  if(!digitalRead(erasePin)) //check if erase button is pressed
  {
    clearEEPROM();
    delay(500);
  }
   
  delay(1);
}
 
void printTemp()
{
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    byte value = EEPROM.read(i);                //read EEPROM data at address i
    if(value != 0)                              //skip "empty" addresses
    {
      float temp = value*conv_coeff;            //convert ADC values to temperature
      temp = (temp - 0.5)*100;                  //take care of the offset
 
      Serial.println(temp);
    }
  }
}
 
void clearEEPROM()
{
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    if(EEPROM.read(i) != 0)                     //skip already "empty" addresses
    {
      EEPROM.write(i, 0);                       //write 0 to address i
    }
  }
  Serial.println("EEPROM erased");
  address = 0;                                  //reset address counter
}
 
void writeTemp()
{
  byte value = analogRead(tempPin);     //read sensor value
   
  EEPROM.write(address, value);         //write value to current address counter address
 
  Serial.print("Sensor value stored at address ");
  Serial.println(address);
   
  address++;                      //increment address counter
  if(address == EEPROM.length())  //check if address counter has reached the end of EEPROM
  {
    address = 0;              //if yes: reset address counter
  }
}
 
  
