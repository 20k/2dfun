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

    //int item_type_id_for_sale = -1;
    std::string item_class;
    int rarity;
    item* specific_object = nullptr;
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

struct place_info
{
    std::string item_class;
    int rarity = -1;
    bool placing = false;

    ///ignore me for the moment
    item* specific_object = nullptr;
};

///literal shop representation
///need to list if items are equipped
///need character -> equip and character -> unequip, and possibly a can_sell tag, or something more useful
///implement drag and drop from shop items to character invents
struct shop
{
    bool view_is_init = false;
    vec2i dim;
    int grid_dim = 0;

    item_manager* item_manage;

    place_info currently_placing;

    std::vector<sellable*> for_sale;

    std::vector<tile> tiles;

    void init(item_manager* imanage, vec2i pdim, int pgrid_dim);

    ///only adds item to item list
    sellable* make_sellable(item* i, float price = -1);

    void remove_sellable(sellable* s);

    void place_sellable(sellable* s, vec2f pos); ///pos is snapped

    void place_selection(vec2f pos);

    ///will need to convert from camera pos to shop pos
    vec2i pos_to_grid_snapped(vec2f pos);

    void draw(draw_manager& draw_manage);

    void draw_shopfront_ui(draw_manager& draw_manage);

    void tick(draw_manager& draw_manage);

    std::vector<sellable*> get_sellable_by_rarity(int rarity);
};

#endif // SHOP_HPP_INCLUDED
