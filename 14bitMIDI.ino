#include <ResponsiveAnalogRead.h>
//#include "usb_name.c"

// number of faders to read from 
#define COUNT 1
// number of bits
#define FOURTEEN 14
// maximum value to normalize input tu
#define RESOLUTION 16383
// input reading point
#define IN A0

// hardcoded values to general purpose midi out, 16 (coarse), 48 (fine)
#define MSB 16
#define LSB 48


// Responsive analog reader
ResponsiveAnalogRead reader(IN, true);

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

void setup() {
  // Set teensy to use 14bit and the analog reader to use 16383 as resolution.
  analogReadResolution(FOURTEEN);
  reader.setAnalogResolution(RESOLUTION);
  Serial.begin(31250);

}


int i = 0;

void loop() {
  reader.update();
  
  // Sample 32 values for each fader
    if (i < 32) {
        for (int j = 0; j < COUNT; ++j) {

        // Insert MULTIPLEX-logic here <--------------------------------------------------
        
        faders[j] += map(reader.getValue(), 1, 16382, 0, 16383);
        }
        i++;
    } else {
        i = 0;
        for (int j = 0; j < COUNT; ++j) {


        // Shift value by 5, "dividing by 32"
        // remove bad values, if negative values or values bigger than 16383
        faders[j] = removeBadValues(faders[j] >> 5);

        // send CC to correct midi destination, sequencial LSB and MSB.
        usbMIDI.sendControlChange(LSB + j, faders[j] & 127, 1);
        usbMIDI.sendControlChange(MSB + j, (faders[j] >> 7) & 127, 1);

        // Will only run if -DDEBUG flag is used at compile time
        D(Serial.printf("VAL: %i\tLSB: %i\tMSB: %i\n", faders[j], (faders[j] & 0x7F), ((faders[j] >> 7) & 0x7F)));

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
