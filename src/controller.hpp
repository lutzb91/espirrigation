#pragma once
#include <Arduino.h>
#include "valve.hpp"

class Controller {
    private:
        int data_pin_ = 12;   //Outputs the byte to transfer
        int load_pin_ = 13;   //Controls the internal transference of data in SN74HC595 internal registers
        int clock_pin_ = 15;  //Generates the clock signal to control the transference of data

        uint16_t num_valves_ = 0;
        Valve *valves_ = NULL;
        uint16_t valves_bitfield_ = 0;
    public:
        Controller(uint8_t num_valves) {
            valves_ = new Valve[num_valves];
        }
        void init(int dataPin = 12, int loadPin = 13, int clockPin = 15);
        void openValve(uint8_t index, uint16_t seconds);
        void closeValve(uint8_t index);
        void updateRegisters();
        void loop();
};