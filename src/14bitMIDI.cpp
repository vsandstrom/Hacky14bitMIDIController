#include "cd74.hpp"
#include <unistd.h>
#include "Arduino.h"
#include <ResponsiveAnalogRead.h>

// number of faders to read from 
#define COUNT 4
// number of bits
// maximum value to normalize input tu
#define RESOLUTION 16383

// hardcoded values to general purpose midi out, 16 (coarse), 48 (fine)
#define MSB 16
#define LSB 48

#define ENABLE 9 
#define SW1 10
#define SW2 11
#define SW3 12
// input reading point
#define IN 14

// Responsive analog reader
ResponsiveAnalogRead reader(IN, true);

Multiplex mux = Multiplex(IN, 0, SW1, SW2, SW3, ENABLE);

int faders[COUNT] = {0};

/*
      // ALPS FADER

    --| READ_OUT                V_IN |--
    --| GND                          |
    --|                              |--
    --|                              |
    
*/

// Turn debug on by using -DDEBUG at compile time. ( -D "#defines" DEBUG )
#ifdef DEBUG
  #define D(x) x
#else
  #define D(x) 
#endif

int removeBadValues(int);

void setup() {
  // Set teensy to use 14bit and the analog reader to use 16383 as resolution.
    mux.initialize();
    analogReadResolution(IN);
    reader.setAnalogResolution(RESOLUTION);
    Serial.begin(31250);

}


int i = 0;

void loop() {

    int j = 1;

  reader.update();

    if (i < 8) {
        // for (int j = 0; j < COUNT; ++j) {

            mux.gate(j);
            delay(15);
            faders[j] += map(reader.getValue(), 1, 16382, 0, 16383);


        //}
        i++;

    } else {
        i = 0;
        // for (int j = 0; j < COUNT; ++j) {

        // Shift value by 5, "dividing by 32"
        // remove bad values, if negative values or values bigger than 16383
        faders[j] = removeBadValues(faders[j] >> 3);

        // send CC to correct midi destination, sequencial LSB and MSB.
        usbMIDI.sendControlChange(LSB + j, faders[j] & 127, 1);
        usbMIDI.sendControlChange(MSB + j, (faders[j] >> 7) & 127, 1);

        // Will only run if -DDEBUG flag is used at compile time
        // D(Serial.printf("FADER%i: %i\tLSB: %i\tMSB: %i\n", j+1, faders[j], (faders[j] & 0x7F), ((faders[j] >> 7) & 0x7F)));
        D(Serial.printf("| %i\t | %i\t | %i\t | %i\t |\n", faders[0], faders[1], faders[2], faders[3]));


        //} 

    }
}

int removeBadValues(int val) {
  if (val < 0) {
    return 0;
  } else if (val > 16383) {
    return 16383;
  } else {
    return val;
  }
}
