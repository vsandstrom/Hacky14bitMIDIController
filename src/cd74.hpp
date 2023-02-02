#include "core_pins.h"

// the Multiplex .read() function should be called after a small delay, letting the signal 
// stabilize while transmitted through the multiplexer.

class Multiplex {
  private:
    int io;
    int mode;
    int sw1, sw2, sw3;
    int enable;
    int sw3_pattern[8] = {0, 0, 0, 0, 1, 1, 1, 1};
    int sw2_pattern[8] = {0, 0, 1, 1, 0, 0, 1, 1};
    int sw1_pattern[8] = {0, 1, 0, 1, 0, 1, 0, 1};
  
  public:
    Multiplex(int io, int mode, int sw1, int sw2, int sw3, int enable);
    void gate(unsigned int gate_num);
    void output_off(); 
    void output_on();
    void initialize();
    int input_read();
};

/// Count binary
///__________________________________
///|  1.  |  2.  |  3.  |    I/O    |
///|  LOW |  LOW |  LOW | 1 active. |
///|  LOW |  LOW | HIGH | 2 active. |
///|  LOW |  LOW | HIGH | 3 active. |
///|________________________________|