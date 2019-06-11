#pragma once
#include <Homie.h>
#include "timer.hpp"

class Valve {
    private:
        bool _open = false;
        uint16_t _duration = 0;
    public:
        void open();
        void close();
        bool isOpen();
        void setDuration(uint16_t duration) {
            _duration = duration;
        }
        uint16_t getDuration() {
            return _duration;
        }
        void loop();
};