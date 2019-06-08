#include "controller.hpp"

void setBit(uint16_t *dest, uint8_t bit, uint8_t value) {
    if(value == 0) {
        *dest &= ~(1 << bit);
    } else {
        *dest |= 1 << bit;
    }
}

void Controller::init(int dataPin, int loadPin, int clockPin) {
    data_pin_ = dataPin;
    load_pin_ = loadPin;
    clock_pin_ = clockPin;
    pinMode(data_pin_, OUTPUT);
    pinMode(load_pin_, OUTPUT);
    pinMode(clock_pin_, OUTPUT);
}

void Controller::updateRegisters() {
    for(int i = 0; i < num_valves_; i++) {
        // Set bit in bitfield is valve is open, otherwise clear bit
        setBit(&valves_bitfield_, i, valves_[i].isOpen() ? 1 : 0);
    }
    // Shift out bits to shift register
    digitalWrite(load_pin_, LOW);
    shiftOut(data_pin_, clock_pin_, MSBFIRST, (valves_bitfield_ >> 8) & 0xFF);
    shiftOut(data_pin_, clock_pin_, MSBFIRST, valves_bitfield_ & 0xFF);
    digitalWrite(load_pin_, HIGH);
}

void Controller::openValve(uint8_t index, uint16_t seconds) {
    valves_[index].open();
}

void Controller::closeValve(uint8_t index){
    valves_[index].close();
}

void Controller::loop() {
    updateRegisters();
}