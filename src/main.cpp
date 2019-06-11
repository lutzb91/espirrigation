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

  if(value == "true") {
    controller.openValve(range.index-1);
  } else {
    controller.closeValve(range.index-1);
  }

  return true;
}

bool valveDurationHandler(const HomieRange& range, const String& value) {
  //check if all characters are digits
  for(int i=0;i<value.length();i++) {
    if(!isDigit(value.charAt(i))) {
      return false;
    }
  }
  int duration = value.toInt();
  controller.setValveDuration(range.index-1, duration);
  return true;
}

bool valveOrderHandler(const HomieRange& range, const String& value) {

}

void valveChanged(int num, Valve& valve) {
  Homie.getLogger() << "Valve " << num << " changed to " << (valve.isOpen() ? "open":"close") << endl;
  valveNode.setProperty("open").setRange(num+1).send(valve.isOpen() ? "true" : "false");
}

void durationChanged(int num, Valve& valve) {
  Homie.getLogger() << "Duration of valve " << num << " changed to " << valve.getDuration() << " seconds" << endl;
  valveNode.setProperty("duration").setRange(num+1).send(String(valve.getDuration()));
}

void setupHandler() {
  controller.setChangedHandler(valveChanged);
  controller.setDurationHandler(durationChanged);
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

  valveNode.advertiseRange("open", 1, NUM_VALVES).settable(valveOpenHandler);
  valveNode.advertiseRange("duration", 1, NUM_VALVES).settable(valveDurationHandler);
  valveNode.advertiseRange("order", 1, NUM_VALVES).settable(valveOrderHandler);

  Homie.setSetupFunction(setupHandler);
  Homie.setLoopFunction(loopHandler);

  Homie.setup();
}

void loop() {
  Homie.loop();
}