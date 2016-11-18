#ifndef SHOP_HPP_INCLUDED
#define SHOP_HPP_INCLUDED

#include <vec/vec.hpp>

struct item;
struct economic_item;
struct item_manager;

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

///literal shop representation
struct shop
{
    vec2i dim;

    item_manager* item_manage;

    std::vector<sellable*> for_sale;

    void init(item_manager* imanage);

    ///only adds item to item list
    sellable* make_sellable(item* i, float price = -1);

    void remove_sellable(sellable* s);
};

#endif // SHOP_HPP_INCLUDED
