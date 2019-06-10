#include "timer.hpp"
#include <Homie.h>

void Timer::startTimer(uint16_t seconds) {
    _last_millis = millis();
    _timer_millis = seconds * 1000; // time in millis
    _running = true;
    //Homie.getLogger() << "Timer gestartet: " << timer_millis_ << " millis" << endl;
}

unsigned long lastMillis = 0;

void Timer::loop() {
    if(_running) {
        unsigned long duration = millis() - _last_millis;
        _last_millis = millis();
        _timer_millis -= duration;
        if(_timer_millis <= 0) {
            _running = false;
            if(_callback != NULL) {
                _callback(this);
            }
        }
    }
}

void Timer::cancel() {
    _running = false;
}