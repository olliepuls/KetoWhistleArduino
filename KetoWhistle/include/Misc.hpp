#define BUTTON_PIN 10
#define TEMP_PIN A3

#include <Arduino.h>
#include <NanoBLEFlashPrefs.h>

extern volatile bool button_interrupt_flag;

void configure_button_interrupt();
void buttonHandler();