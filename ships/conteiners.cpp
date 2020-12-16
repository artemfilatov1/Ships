#include "conteiners.hpp"

namespace generate_ship{
    ship* create_ship(){
        ship* tmp = new ship;
        tmp->type = type_ship(rand() % 3);
        tmp->tonnage = (rand() % 3)*5;
        if (tmp->tonnage == 0) tmp->tonnage = 1;
        tmp->l = Tunnel_in;
        return tmp;
    }
};
