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

    bool grabbing = false;
    sellable* grabbed = nullptr;

    bool peon_window_hovered = false;
    int grab_c = 2;

    int entity_num_hovered = -1;

    void tick_entity_grab(entity_manager& entity_manage, shop& s);
    void tick();
};

#endif // DRAG_MANAGER_HPP_INCLUDED
