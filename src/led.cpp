#include "Header/led.h"

LED::LED(int pin) {
  this->pin = pin;

  status = false;
  pattern = 0;
}

LED::~LED() {}

void LED::toggle() {
  if(status) {
    off();
  } else {
    on();
  }
}

void LED::on() {
  digitalWrite(pin, HIGH);
  steady = true;
  status = true;
}

void LED::off() {
  digitalWrite(pin, LOW);
  steady = true;
  status = false;
}

void LED::setPattern(int pattern) {
  steady = false;
}

void LED::cycle() {
  if(!steady) {
    timer++;

    if (timer >= patterns[pattern]) {

    }
  }
}