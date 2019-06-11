#pragma once
#include <Arduino.h>
#include <functional>
#include "valve.hpp"
#include "timer.hpp"

class Controller {
    private:
        int data_pin_ = 12;   //Outputs the byte to transfer
        int load_pin_ = 13;   //Controls the internal transference of data in SN74HC595 internal registers
        int clock_pin_ = 15;  //Generates the clock signal to control the transference of data

        uint16_t _num_valves = 0;
        Valve *_valves = NULL;
        Timer *_timers = NULL;
        uint16_t valves_bitfield_ = 0;
        uint16_t valves_bitfield_old_ = 1; // set different, so that shift out is called at startup
        void (*_callback)(int num, Valve& valve) = NULL;
        void (*_durationCallback)(int num, Valve& valve) = NULL;

        void timerFinished(Timer *timer);
    public:
        Controller(uint8_t num_valves) {
            _valves = new Valve[num_valves];
            _timers = new Timer[num_valves];
            _num_valves = num_valves;
        }
        void init(int dataPin = 12, int loadPin = 13, int clockPin = 15);
        void openValve(uint8_t index);
        void closeValve(uint8_t index);
        void setValveDuration(uint8_t index, uint16_t seconds);
        void updateRegisters();
        void loop();

        void setChangedHandler(void (*callback)(int num, Valve& valve)) {
            _callback = callback;
        }

        void setDurationHandler(void (*callback)(int num, Valve& valve)) {
            _durationCallback = callback;
        }
        
};