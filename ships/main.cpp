#include <iostream>
#include <thread>
#include <future>
#include "check_state.hpp"
#include "timer.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    timer t;
    check_state ch_s;
    t.ch_s = &ch_s;
    ch_s.tm = &t;
    cout << "time for tunnel: "; cin >> ch_s.time_for_tunnel;
    thread th([&ch_s](void){ch_s.run();});
    t.run();
    th.join();
    return 0;
}
