#include <ResponsiveAnalogRead.h>
//#include "usb_name.c"

// number of faders to read from 
#define COUNT 1

int fader0 = A0;

int val0 = 0;
int prevVal = 0;
int diff = 0;
int maxDiff = 0;
int MSB = 16;
int LSB = MSB+32;
int i = 0;

// Responsive analog reader
ResponsiveAnalogRead fAnalog0(fader0, true);

int faders[COUNT] = {0};

/*

      // ALPS FADER

    --| READ_OUT                V_IN |--
    --| GND                          |
    --|                              |--
    --|                              |
*/

#define DEBUG(val, lsb, msb) //Serial.printf("VAL: %i\tLSB: %i\tMSB: %i\n", val, lsb, msb);
#define DIFF_DEBUG(val, prev_val, diff, max_diff) // diff = abs(val0 - prev_val); if (diff > maxDiff) {maxDiff = diff; Serial.printf("diff: %i\n", maxDiff);}


void setup() {
  // Set teensy to use 14bit and the analog reader to use 16383 as resolution.
  analogReadResolution(14);
  fAnalog0.setAnalogResolution(16383);
  Serial.begin(31250);

}

void loop() {
  fAnalog0.update();
  
  // Sample 32 values
  if (i < 32) {
    i++;

    for (int j = 0; j < COUNT; ++j) {


      // Insert MULTIPLEX-logic here <--------------------------------------------------


      faders[j] += map(fAnalog0.getValue(), 1, 16382, 0, 16383);
    }
  } else {
    i = 0;
    

    for (int j = 0; j < COUNT; ++j) {


      // Shift value by 5, "dividing by 32"
      // remove bad values, if negative values or values bigger than 16383
      faders[j] = removeBadValues(faders[j] >> 5);

      // send CC to correct midi destination, sequencial LSB and MSB.
      usbMIDI.sendControlChange(LSB + j, faders[j] & 127, 1);
      usbMIDI.sendControlChange(MSB + j, (faders[j] >> 7) & 127, 1);

    } 

    delay(1);

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
