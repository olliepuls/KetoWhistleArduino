#include <Misc.hpp>

volatile bool button_interrupt_flag = false;

void configure_button_interrupt() {
    attachInterrupt(digitalPinToInterrupt(10), buttonHandler, FALLING);
}

void buttonHandler(){
    button_interrupt_flag = true;
    //Serial.println("Pressed Button");
}