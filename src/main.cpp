#include <Arduino.h>
#include <Homie.h>

#include "controller.hpp"

const int PIN_RELAY = 5;
const int NUM_VALVES = 16;

const int dataPin = 12;   //Outputs the byte to transfer
const int loadPin = 13;   //Controls the internal transference of data in SN74HC595 internal registers
const int clockPin = 15;  //Generates the clock signal to control the transference of data

HomieNode valveNode("valve", "switch");

Controller controller(NUM_VALVES);

bool valveOpenHandler(const HomieRange& range, const String& value) {
  if (value != "true" && value != "false") return false;

  Homie.getLogger() << "Valve " << range.index << " changed to " << (value ? "open":"close") << endl;
  bool open = (value == "true");
  
  if(open) {
    controller.openValve(range.index, 10);
  } else {
    controller.closeValve(range.index);
  }

  return true;
}

void valveChanged(int num, Valve& valve) {
  valveNode.setProperty("open").setRange(num).send(valve.isOpen() ? "true" : "false");
}

void setupHandler() {
  controller.setChangedHandler(valveChanged);
  controller.init();
}

void loopHandler() {
  controller.loop();
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;
  WiFi.disconnect();
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, LOW);

  Homie_setBrand("EspIrrigation");
  Homie_setFirmware("espirrigation", "0.0.1");

  valveNode.advertiseRange("open", 0, 15).settable(valveOpenHandler);

  Homie.setSetupFunction(setupHandler);
  Homie.setLoopFunction(loopHandler);

  Homie.setup();
}

void loop() {
  Homie.loop();
}