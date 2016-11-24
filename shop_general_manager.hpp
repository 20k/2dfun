#ifndef SHOP_GENERAL_MANAGER_HPP_INCLUDED
#define SHOP_GENERAL_MANAGER_HPP_INCLUDED

#include "shop.hpp"

struct draw_manager;

struct shop_general_manager
{
    bool new_day = false;

    float time_s = -1;
    shop shop_manage;
    item_manager* item_manage;

    void init(item_manager* it, vec2i dim, int grid_size);

    void tick(float dt_s, draw_manager& draw_manage); ///for world transforms we need draw_manager

    void draw_tiles(draw_manager& draw_manage);
    void draw_shop_ui(draw_manager& draw_manage, drag_manager& drag_manage);

    bool is_new_day();
};

#endif // SHOP_GENERAL_MANAGER_HPP_INCLUDED
