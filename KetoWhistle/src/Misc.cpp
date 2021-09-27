#include <Misc.hpp>

volatile bool button_interrupt_flag = false;

void configure_button_interrupt() {
    attachInterrupt(digitalPinToInterrupt(10), buttonHandler, FALLING);
}

void buttonHandler(){
    button_interrupt_flag = true;
    //Serial.println("Pressed Button");
}

typedef struct pastMeasurementStruct {
    float measurements[10];
    int num_recorded;
} pastMeasures;

void write_measurement_to_flash(NanoBLEFlashPrefs *myFlashPrefs, float reading) {
    pastMeasures past;
    myFlashPrefs->readPrefs(&past, sizeof(past));

    if (past.num_recorded >= 10) {
        for (int i = 1; i < 10; i++) {
            past.measurements[i-1] = past.measurements[i];
        }
        past.measurements[9] = reading;
    } else {
        past.measurements[past.num_recorded] = reading;
    }

    past.num_recorded++;
    myFlashPrefs->writePrefs(&past, sizeof(past));
}


