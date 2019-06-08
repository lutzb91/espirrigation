#pragma once

class Valve {
    private:
        bool open_ = false;
    public:
        void open();
        void close();
};