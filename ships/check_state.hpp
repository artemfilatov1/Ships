#ifndef check_state_hpp
#define check_state_hpp

#include <stdio.h>
#include <thread>
#include <future>
#include <deque>
#include "timer.hpp"
#include "conteiners.hpp"

class check_state{
    std::mutex m;
    std::condition_variable c_v;
    std::deque<interval> intervals;
public:
    long time_for_tunnel;
    timer* tm;
    void run();
    void add_interval(const interval& i);
};

#endif /* check_state_hpp */
