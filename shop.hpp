#ifndef SHOP_HPP_INCLUDED
#define SHOP_HPP_INCLUDED

#include <vec/vec.hpp>
#include "peon_manager.hpp"

struct item;
struct economic_item;
struct item_manager;
struct draw_manager;
struct entity_manager;

namespace tile_info
{
    enum stile
    {
        FLOOR,
        TABLE_WITH_ITEM,
        WALL,
        DOOR
    };
}

struct tile
{
    vec2i array_pos;

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
    bool locked = false;
};

struct place_info
{
    std::string item_class;
    int rarity = -1;
    bool placing = false;

    ///ignore me for the moment
    item* specific_object = nullptr;
};

struct drag_manager;

///literal shop representation
///need to list if items are equipped
///need character -> equip and character -> unequip, and possibly a can_sell tag, or something more useful
///implement drag and drop from shop items to character invents
struct shop
{
    float money = 0;

    peon_manager peon_manage;

    bool view_is_init = false;
    vec2i dim;
    int grid_dim = 0;

    item_manager* sold_items;
    std::vector<sellable*> sold_sellables;

    item_manager* item_manage;

    place_info currently_placing;

    std::vector<sellable*> for_sale;

    std::vector<tile> tiles;

    void init(item_manager* imanage, vec2i pdim, int pgrid_dim);

    ///only adds item to item list
    sellable* make_sellable(item* i, float price = -1);

    void destroy_sellable(sellable* s);
    void remove_sellable(sellable* s); ///no delete, transfer between sellable lists

    void place_sellable(sellable* s, vec2f pos); ///pos is snapped

    void place_selection(vec2f pos);

    ///will need to convert from camera pos to shop pos
    vec2i pos_to_grid_snapped(vec2f pos);

    void do_character_entity_grab(entity_manager& entity_manage, drag_manager& drag_manage);

    void draw(draw_manager& draw_manage);

    void draw_shopfront_ui(draw_manager& draw_manage, drag_manager& drag_manage);
    void draw_shopinfo_ui(draw_manager& draw_manage);

    //void tick_draw(draw_manager& draw_manage);

    void tick(draw_manager& draw_manage);

    std::vector<sellable*> get_sellable_by_rarity(int rarity);

    vec2i sellable_to_tile(sellable* s);

    void purchase(sellable* s);

    ///returns true and spends price if we can
    ///otherwise returns false
    bool do_purchase(float price);

    void spawn_random_peon();
    void spawn_peon_of_tier(int tier);

    std::vector<tile> get_table_tiles();

    std::vector<sellable*> get_purchasable_sellables_on_tables();

    vec2f get_door_world_pos();

    int get_num_non_leaving_peons();

    float get_total_value_sold();
    float get_avg_rarity_of_last_n(int n);
};

#endif // SHOP_HPP_INCLUDED
