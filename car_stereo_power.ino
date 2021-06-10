#include <LowPower.h>

#define OUTPUT_PIN 4
#define ACC_PIN 3

bool readACC() {
  return !digitalRead(ACC_PIN);
}

void enableOutput() {
  // Gate at ground
  pinMode(OUTPUT_PIN, OUTPUT);
  digitalWrite(OUTPUT_PIN, LOW);
}

void disableOutput() {
  // Gate at Hi-Z
  pinMode(OUTPUT_PIN, INPUT);
}

void interrupt_acc_on() {
  enableOutput();
}

void interrupt_acc_off() {
  auto delay_2s = millis();

  while (millis() - delay_2s < 2000) {
    if (readACC()) {
      // car is back on, do nothing
      return;
    }
  }

  // turn the stereo off
  disableOutput();
}

bool run_off_interrupt = false;
bool run_on_interrupt = false;

void interrupt_acc() {
  digitalWrite(LED_BUILTIN, HIGH);
  if (readACC())
    run_on_interrupt = true;
  else
    run_off_interrupt = true;
}

void setup() {
  run_off_interrupt = false;
  run_on_interrupt = false;
  enableOutput();
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ACC_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ACC_PIN), interrupt_acc, CHANGE);
}

void loop() {
  if (run_off_interrupt) {
    interrupt_acc_off();
    run_off_interrupt = false;
    digitalWrite(LED_BUILTIN, LOW);
  }

  if (run_on_interrupt) {
    interrupt_acc_on();
    run_on_interrupt = false;
    digitalWrite(LED_BUILTIN, LOW);
  }

  if (!run_on_interrupt && !run_off_interrupt) {
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  }
}