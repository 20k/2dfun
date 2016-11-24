#include "shop_general_manager.hpp"
#include "stats.hpp"

void shop_general_manager::init(item_manager* it, vec2i dim, int grid_size)
{
    shop_manage.init(it, dim, grid_size);
}

void do_day_progression(shop_general_manager& shop_general)
{
    int peons_per_day = 20;

    if(shop_general.is_new_day())
    {
        shop_general.peons_left_to_spawn += peons_per_day;
    }

    ///wtf
    ///we need to stagger these random peons
    int peon_count = shop_general.shop_manage.get_num_non_leaving_peons();

    if(peon_count < stats::peon_minimum_count && shop_general.peons_left_to_spawn < 2)
    {
        shop_general.peons_left_to_spawn++;
    }

    if(shop_general.peons_left_to_spawn > 0 && shop_general.peon_spawn_stagger_elapsed_s > stats::peon_spawn_time_stagger)
    {
        shop_general.shop_manage.spawn_random_peon();

        shop_general.peon_spawn_stagger_elapsed_s = 0.f;

        shop_general.peons_left_to_spawn--;
    }
}

void shop_general_manager::tick(float dt_s, draw_manager& draw_manage)
{
    shop_manage.tick(draw_manage);

    do_day_progression(*this);

    float day_current = floor(time_s / stats::day_len_s);
    float day_next = floor((time_s + dt_s) / stats::day_len_s);

    time_s += dt_s;

    new_day = false;

    if(day_current != day_next)
    {
        new_day = true;
    }

    peon_spawn_stagger_elapsed_s += dt_s;

    //printf("%f %f %f\n", day_current, day_next, time_s);

    //printf("%i\n", (int)day_current);
}

void shop_general_manager::draw_tiles(draw_manager& draw_manage)
{
    shop_manage.draw(draw_manage);
}

void shop_general_manager::draw_shop_ui(draw_manager& draw_manage, drag_manager& drag_manage)
{
    shop_manage.draw_shopfront_ui(draw_manage, drag_manage);
    shop_manage.draw_shopinfo_ui(draw_manage);
}

bool shop_general_manager::is_new_day()
{
    return new_day;
}
