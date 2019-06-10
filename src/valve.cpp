#include "valve.hpp"

void Valve::open() {
    _open = true;
}

void Valve::close() {
    _open = false;
}

bool Valve::isOpen() {
    return _open;
}

void Valve::loop() {
}