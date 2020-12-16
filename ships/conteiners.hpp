#ifndef conteiners_hpp
#define conteiners_hpp

#include <stdio.h>
#include <list>
#include <deque>

enum location{
    Tunnel_in, Tunnel_out, Bay_in, Bay_out, Berth
};

enum type_ship{
    first, second, third
};

struct ship{
    type_ship type;
    unsigned int tonnage;
    location l;
};

struct berth{
    ship* owner;
    type_ship type;
};

struct bay{
    std::deque<ship*> ships;
};

struct tunnel{
    ship* ships[5];
    int cnt_ships;
};

struct interval{
    ship* owner;
    long time;
};

namespace generate_ship {
    ship* create_ship();
}

#endif /* conteiners_hpp */
