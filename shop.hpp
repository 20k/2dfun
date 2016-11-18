#ifndef SHOP_HPP_INCLUDED
#define SHOP_HPP_INCLUDED

#include <vec/vec.hpp>

struct item;
struct economic_item;
struct item_manager;
struct draw_manager;

namespace tile_info
{
    enum stile
    {
        FLOOR,
        TABLE_WITH_ITEM,
        WALL
    };
}

struct tile
{
    tile_info::stile tile_type;

    int item_type_id_for_sale = -1;
};

struct sellable
{
    float listed_price = 0.f;
    item* i = nullptr;
};

struct peon
{
    float wallet = 0.f;
    ///specific weapon interest
    ///happiness
    ///path position random
    ///calculate environmental modifiers
};

///literal shop representation
struct shop
{
    bool view_is_init = false;
    vec2i dim;
    int grid_dim = 0;

    item_manager* item_manage;

    std::vector<sellable*> for_sale;

    void init(item_manager* imanage, vec2i pdim, int pgrid_dim);

    ///only adds item to item list
    sellable* make_sellable(item* i, float price = -1);

    void remove_sellable(sellable* s);

    void place_sellable(sellable* s, vec2i pos);

    ///will need to convert from camera pos to shop pos
    vec2i pos_to_grid_snapped(vec2f pos);

    void draw(draw_manager& draw_manage);
};

#endif // SHOP_HPP_INCLUDED
