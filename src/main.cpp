#include <Arduino.h>
#include <Homie.h>

#include "valve.hpp"

const int PIN_RELAY = 5;
const int NUM_VALVES = 16;

const int dataPin = 12;   //Outputs the byte to transfer
const int loadPin = 13;   //Controls the internal transference of data in SN74HC595 internal registers
const int clockPin = 15;  //Generates the clock signal to control the transference of data

HomieNode lightNode("light", "switch");

HomieNode valveNode("valve", "switch");

uint16_t ventile = 0;

Valve valves[NUM_VALVES];

bool valveOpenHandler(const HomieRange& range, const String& value) {
  if (value != "true" && value != "false") return false;

  Homie.getLogger() << "Valve " << range.index << " changed to " << (value ? "open":"close") << endl;
  bool open = (value == "true");
  
  if(open) {
    valves[range.index].open();
  } else {
    valves[range.index].close();
  }

  valveNode.setProperty("open").setRange(range).send(value);
  return true;
}

void setBit(uint8_t bit, uint8_t value) {
    if(value == 0) {
        ventile &= ~(1 << bit);
    } else {
        ventile |= 1 << bit;
    }
}

void activateVentil(uint8_t num) {
  setBit(num, 1);
  digitalWrite(loadPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, (ventile >> 8) & 0xFF);
  shiftOut(dataPin, clockPin, MSBFIRST, ventile & 0xFF);
  digitalWrite(loadPin, HIGH);
}

void deactivateVentil(uint8_t num) {
  setBit(num, 0);
  digitalWrite(loadPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, (ventile >> 8) & 0xFF);
  shiftOut(dataPin, clockPin, MSBFIRST, ventile & 0xFF);
  digitalWrite(loadPin, HIGH);
}

void initialize() {
  pinMode(dataPin, OUTPUT);
  pinMode(loadPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  byte data = B00000000;

  digitalWrite(loadPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, data);
  shiftOut(dataPin, clockPin, MSBFIRST, data);
  digitalWrite(loadPin, HIGH);
}

unsigned long lastMillis = 0;

uint8_t current = 0;

void loop2() {
  if (millis() - lastMillis >= 1000UL) {
    deactivateVentil(current);
    current = (current + 1) % 16;
    Serial << "Current: " << current << endl;
    activateVentil(current);
    lastMillis = millis();
  }
}

void setup() {
  Serial.begin(9600);
  Serial << endl << endl;
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, LOW);

  Homie_setFirmware("espirrigation", "0.0.1");

  valveNode.advertiseRange("open", 0, 15).settable(valveOpenHandler);

  Homie.setSetupFunction(initialize);
  Homie.setLoopFunction(loop2);

  Homie.setup();
}

void loop() {
  Homie.loop();
}