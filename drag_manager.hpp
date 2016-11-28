#ifndef DRAG_MANAGER_HPP_INCLUDED
#define DRAG_MANAGER_HPP_INCLUDED

#include <vec/vec.hpp>
#include "peon_manager.hpp"
#include "shared.hpp"

struct item;
struct economic_item;
struct item_manager;
struct draw_manager;
struct entity_manager;

struct sellable;

///for ui dragging
///put entity grab in here?
struct drag_manager
{
    //bool clicked = false;
    //bool dragging = false;

    input_state st;

    bool currently_levelup_clicked = false; ///we've clicked once on a level up thing
    bool grabbing_sellable = false;
    bool grabbing_item = false;
    sellable* grabbed_sellable = nullptr;
    item* grabbed_item = nullptr;

    bool shopfront_window_hovered = false;

    bool peon_window_hovered = false;
    int grab_c = 2;
    //int grab_timer = 2; ///min 2 frames of grab before we can do stuff

    vec2f entity_window_pos = {0,0};
    vec2f last_entity_window_pos = {0,0};

    int entity_num_hovered = -1; ///which entity number
    int entity_column_hovered = -1; ///which part of the entity
    int entity_individual_hovered = -1; ///which individual element is hovered over

    int saved_entity_num = -1;
    int saved_entity_column = -1;

    int saved_stat_item = -1;

    bool grab_sellable(sellable* s);
    bool grab_item(item* i);

    void tick_entity_grab(entity_manager& entity_manage, shop& s);
    void tick();

    bool sellable_is_grabbed();
    bool item_is_grabbed();

    bool any_grabbed();

    bool hovering_over_any_entity();
    bool hovering_over_specific_entity_column();
    bool hovering_over_shopfront_window();
    bool hovering_over_individual_entity_item();

    ///specific_entity_column - 5
    //int get_inventory_item_id();

    int column_id_to_invent(int col_id);
    int entity_individual_to_invent_id(int val);

    void ungrab();

    bool carrying_sellable(sellable* s);

    void force_peons_unseek_current_sellable(shop& s);

    void update_entity_window_pos(vec2f p);

    void release_sellable_lock();
};

#endif // DRAG_MANAGER_HPP_INCLUDED
