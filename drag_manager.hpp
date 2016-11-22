#ifndef DRAG_MANAGER_HPP_INCLUDED
#define DRAG_MANAGER_HPP_INCLUDED

#include <vec/vec.hpp>
#include "peon_manager.hpp"

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
    bool clicked = false;
    bool dragging = false;

    bool grabbing_sellable = false;
    bool grabbing_item = false;
    sellable* grabbed_sellable = nullptr;
    item* grabbed_item = nullptr;

    bool shopfront_window_hovered = false;

    bool peon_window_hovered = false;
    int grab_c = 2;

    int entity_num_hovered = -1;

    void grab_sellable(sellable* s);
    void grab_item(item* i);

    void tick_entity_grab(entity_manager& entity_manage, shop& s);
    void tick();

    bool sellable_is_grabbed();
    bool item_is_grabbed();

    bool any_grabbed();
};

#endif // DRAG_MANAGER_HPP_INCLUDED
