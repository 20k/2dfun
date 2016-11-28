#ifndef SHOP_GENERAL_MANAGER_HPP_INCLUDED
#define SHOP_GENERAL_MANAGER_HPP_INCLUDED

#include "shop.hpp"

struct draw_manager;

///handles interaction between world and shop
struct shop_general_manager
{
    float peon_spawn_stagger_elapsed_s = 0.f;

    int peons_left_to_spawn = 0;

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
