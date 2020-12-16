#ifndef timer_hpp
#define timer_hpp

#include <stdio.h>
#include <thread>
#include <future>
#include "conteiners.hpp"

class check_state;

class timer{
    std::mutex m;
    std::list<interval> intervals;
    bool ok;
public:
    timer();
    check_state* ch_s;
    void add_interval(const interval& i);
    void run();
    void stop();
};

#endif /* timer_hpp */
