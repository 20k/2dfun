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
        for(int i=0; i<peons_per_day; i++)
        {
            shop_general.shop_manage.spawn_random_peon();
        }
    }
}

void shop_general_manager::tick(float dt_s, draw_manager& draw_manage)
{
    shop_manage.tick(draw_manage);

    do_day_progression(*this);

    float day_current = floor(fmod(time_s, stats::day_len_s) / stats::day_len_s);
    float day_next = floor(fmod(time_s + dt_s, stats::day_len_s) / stats::day_len_s);

    time_s += dt_s;

    new_day = false;

    if(day_current != day_next)
    {
        new_day = true;
    }

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
