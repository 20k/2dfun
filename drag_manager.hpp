#ifndef DRAG_MANAGER_HPP_INCLUDED
#define DRAG_MANAGER_HPP_INCLUDED

struct sellable;

///for ui dragging
struct drag_manager
{
    bool clicked = false;
    bool dragging = false;

    bool grabbing = false;
    sellable* grabbed = nullptr;

    bool peon_window_hovered = false;
    int grab_c = 2;


    int entity_num_hovered = -1;
};

#endif // DRAG_MANAGER_HPP_INCLUDED
