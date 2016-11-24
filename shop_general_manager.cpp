#include "shop_general_manager.hpp"

void shop_general_manager::init(item_manager* it, vec2i dim, int grid_size)
{
    shop_manage.init(it, dim, grid_size);
}

void shop_general_manager::tick(float dt_s, draw_manager& draw_manage)
{
    shop_manage.tick(draw_manage);
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
