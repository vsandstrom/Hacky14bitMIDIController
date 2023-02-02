#include "cd74.hpp"

void Multiplex::gate(unsigned int gate_num) {
  int num = 0;
  if (gate_num > 0 && gate_num < 8) num = gate_num; 
  digitalWrite(sw1, sw1_pattern[num]);
  digitalWrite(sw2, sw2_pattern[num]);
  digitalWrite(sw3, sw3_pattern[num]);

}

void Multiplex::output_off(){
  digitalWrite(io, 0);
}

void Multiplex::output_on() {
  digitalWrite(io, 1);
}

void Multiplex::initialize() {
  pinMode(sw1, OUTPUT);
  pinMode(sw2, OUTPUT);
  pinMode(sw3, OUTPUT);
  pinMode(enable, OUTPUT);
  pinMode(io, mode);
  digitalWrite(enable, LOW);
}

int Multiplex::input_read() {
  return io;
}

Multiplex::Multiplex(int io, int mode, int sw1, int sw2, int sw3, int enable) : io(io), mode(mode), sw1(sw1), sw2(sw2), sw3(sw3), enable(enable){};
