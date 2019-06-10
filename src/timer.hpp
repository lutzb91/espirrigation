#pragma once
#include <Arduino.h>
#include <functional>

class Timer {
    private:
        unsigned long _last_millis = 0;
        uint32_t _timer_millis = 0;
        bool _running = false;
        std::function<void(Timer*)> _callback = NULL;
    public:
        void startTimer(uint16_t seconds);
        void loop();
        void cancel();
        void setCallback(std::function<void(Timer*)> callback) {
            _callback = callback;
        }
};