#include "timer.hpp"
#include "check_state.hpp"
#include <thread>
#include <sys/time.h>

using namespace std;

void timer::add_interval(const interval& i){
    lock_guard<mutex> lg(m);
    intervals.push_back(i);
}

void timer::run(){
    timeval s_time;
    gettimeofday(&s_time, NULL);
    long millis = (s_time.tv_sec * 1000) + (s_time.tv_usec / 1000);
    while (ok) {
        list<interval> tmp;
        {
            lock_guard<mutex> lg(m);
            for (list<interval>::iterator it = intervals.begin(); it!=intervals.end();){
                if (it->time >= millis){
                    tmp.push_back(*it);
                    it = intervals.erase(it);
                } else {
                    it++;
                }
            }
        }
        for (auto it : tmp)
            ch_s->add_interval(it);
        
        
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

timer::timer(): ok(true){}

void timer::stop(){
    ok = false;
}
