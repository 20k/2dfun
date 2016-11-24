#ifndef SHOP_GENERAL_MANAGER_HPP_INCLUDED
#define SHOP_GENERAL_MANAGER_HPP_INCLUDED

#include "shop.hpp"

struct shop_general_manager
{
    float time_s = 0;
    shop s;

    void tick(float dt_s);
};

#endif // SHOP_GENERAL_MANAGER_HPP_INCLUDED
