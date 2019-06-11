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

    for(int i=0;i<_num_valves;i++) {
        _timers[i].setCallback(std::bind(&Controller::timerFinished, this, std::placeholders::_1));
    }
}

void Controller::updateRegisters() {
    for(int i = 0; i < _num_valves; i++) {
        // Set bit in bitfield is valve is open, otherwise clear bit
        setBit(&valves_bitfield_, i, _valves[i].isOpen() ? 1 : 0);
    }
    if(valves_bitfield_old_ != valves_bitfield_) {
        // Shift out bits to shift register
        digitalWrite(load_pin_, LOW);
        shiftOut(data_pin_, clock_pin_, MSBFIRST, (valves_bitfield_ >> 8) & 0xFF);
        shiftOut(data_pin_, clock_pin_, MSBFIRST, valves_bitfield_ & 0xFF);
        digitalWrite(load_pin_, HIGH);
        valves_bitfield_old_ = valves_bitfield_;
    }
}

void Controller::openValve(uint8_t index) {
    _valves[index].open();
    _timers[index].startTimer(_valves[index].getDuration());
    if(_callback != NULL) {
        _callback(index, _valves[index]);
    }
}

void Controller::closeValve(uint8_t index){
    _timers[index].cancel();
    _valves[index].close();
    if(_callback != NULL) {
        _callback(index, _valves[index]);
    }
}

void Controller::setValveDuration(uint8_t index, uint16_t seconds) {
    _valves[index].setDuration(seconds);
    if(_durationCallback != NULL) {
        _durationCallback(index, _valves[index]);
    }
}

void Controller::timerFinished(Timer *timer) {
    // Search corresponding timer
    for(int i=0;i<_num_valves;i++) {
        if(&_timers[i] == timer) {
            closeValve(i);
        }
    }
}

void Controller::loop() {
    // update valves and timers
    for(int i = 0; i< _num_valves; i++) {
        _valves[i].loop();
        _timers[i].loop();
    }
    updateRegisters();
}