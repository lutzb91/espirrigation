#pragma once
#include <Homie.h>
#include "timer.hpp"

class Valve {
    private:
        bool _open = false;
    public:
        void open();
        void close();
        bool isOpen();
        void loop();
};