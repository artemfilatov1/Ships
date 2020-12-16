#include "check_state.hpp"
#include <vector>
#include <sys/time.h>

using namespace std;

static inline void berth_state(berth& Berth, bay& Bay, timer& t){
    for (auto it = Bay.ships.begin(); it != Bay.ships.end(); it++){
        if ((*it)->type == Berth.type){
            Berth.owner = *it;
            (*it)->l = location::Berth;
            
            timeval s_time;
            gettimeofday(&s_time, NULL);
            long millis = (s_time.tv_sec * 1000) + (s_time.tv_usec / 1000);
            interval tmp{.time = millis+(*it)->tonnage*1000, .owner = *it};
            t.add_interval(tmp);
            
            Bay.ships.erase(it);
            break;
        }
    }
}

static void print_inf(const tunnel& t, const berth& b1, const berth& b2, const berth& b3, const bay& b){
    printf("count ships of bay: %lu\n", b.ships.size());
    printf("count ships of tunnel: %u\n", t.cnt_ships);
    unsigned int cnt_in = 0, cnt_out = 0;
    for (auto &it : b.ships){
        if (it->l == Bay_in) cnt_in++;
        if (it->l == Bay_out) cnt_out++;
    }
    printf("count spips waiting berth: %u\n", cnt_in);
    printf("count spips waiting tunnel: %u\n", cnt_out);
}

void check_state::run(){
    timeval s_time;
    gettimeofday(&s_time, NULL);
    long millis = (s_time.tv_sec * 1000) + (s_time.tv_usec / 1000);
    interval inf{.owner = 0, .time = millis+1000};
    tm->add_interval(inf);
    
    tunnel t{0};
    berth b1{.owner = 0, .type = first};
    berth b2{.owner = 0, .type = second};
    berth b3{.owner = 0, .type = third};
    bay b;
    while (true) {
        //оброботка туннеля
        vector<ship*> v;
        int cnt1 = 2-t.cnt_ships/2;
        int cnt2 = 5-t.cnt_ships - cnt1;
        while (cnt2 != 0){
            if (b.ships.size()!=0){
                v.push_back(b.ships.front());
                v.back()->l = Tunnel_out;
                b.ships.pop_front();
            } else {
                cnt1++;
            }
            cnt2--;
        }
        while (cnt1!=0) {
            v.push_back(generate_ship::create_ship());
            cnt1--;
        }
        
        timeval s_time;
        gettimeofday(&s_time, NULL);
        long millis = (s_time.tv_sec * 1000) + (s_time.tv_usec / 1000);
        
        for (auto & t_ship : v){
            t.ships[t.cnt_ships] = t_ship;
            t.cnt_ships++;
            interval i{.time = millis+time_for_tunnel, .owner = t_ship};//TODO: время прохода взять при старте прогр
            tm->add_interval(i);
        }
        //обр событей
        {
            lock_guard<mutex> lg(m);
            while (intervals.size() != 0){
                interval i = intervals.front();
                intervals.pop_front();
                if (i.owner == 0){
                    print_inf(t, b1, b2, b3, b);
                    timeval s_time;
                    gettimeofday(&s_time, NULL);
                    long millis = (s_time.tv_sec * 1000) + (s_time.tv_usec / 1000);
                    interval inf{.owner = 0, .time = millis+1000};
                    tm->add_interval(inf);
                    
                }
                
                else if (i.owner->l == Tunnel_out){
                    for (int j = 0; j < 5; j++){
                        if (i.owner == t.ships[j]){
                            t.cnt_ships--;
                            memcpy(&t.ships[j], &t.ships[j+1], sizeof(ship*)*(5-j));//?
                            break;
                        }
                    }
                    delete i.owner;
                }
                else if (i.owner->l == Tunnel_in){
                    i.owner->l = Bay_in;
                    for (int j = 0; j < 5; j++){
                        if (i.owner == t.ships[j]){
                            t.cnt_ships--;
                            memcpy(&t.ships[j], &t.ships[j+1], sizeof(ship*)*(5-j));//?
                            break;
                        }
                    }
                    b.ships.push_back(i.owner);
                }
                else if (i.owner->l == Berth){
                    i.owner->l = Bay_out;
                    if (i.owner->type == first) b1.owner = 0;
                    else if (i.owner->type == second) b2.owner = 0;
                    else if (i.owner->type == third) b3.owner = 0;
                    b.ships.push_back(i.owner);
                }
            }
        }
        //обр bay
        if (b1.owner == 0) berth_state(b1, b, *tm);
        if (b2.owner == 0) berth_state(b2, b, *tm);
        if (b3.owner == 0) berth_state(b3, b, *tm);
        
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void check_state::add_interval(const interval& i){
    lock_guard<mutex> lg(m);
    intervals.push_back(i);
}
