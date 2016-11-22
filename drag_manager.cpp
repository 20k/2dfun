#include "drag_manager.hpp"
#include "stats.hpp"
#include "item.hpp"
#include "inventory.hpp"
#include "entities.hpp"
#include "shop.hpp"
#include <imgui/imgui.h>

void drag_manager::grab_sellable(sellable* s)
{
    grabbed_sellable = s;
    grabbing_sellable = true;
    grab_c = 2;
}

void drag_manager::grab_item(item* i)
{
    grabbed_item = i;
    grabbing_item = true;
    grab_c = 2;
}

void drag_manager::tick_entity_grab(entity_manager& entity_manage, shop& s)
{
    if(grabbing_sellable)
    {
        ImGui::SetTooltip(grabbed_sellable->i->display().c_str());
    }

    sf::Mouse mouse;

    bool left = mouse.isButtonPressed(sf::Mouse::Left);

    if(!left && hovering_over_any_entity() && sellable_is_grabbed())
    {
        if(entity_num_hovered >= entity_manage.chars.size())
        {
            printf("invalid entity num\n");
            return;
        }

        character* c = entity_manage.chars[entity_num_hovered];

        bool success = c->add_to_invent(grabbed_sellable->i);

        if(success)
        {
            grabbed_sellable->locked = false;
            s.remove_sellable(grabbed_sellable);
            s.peon_manage.check_peon_release_sellable(grabbed_sellable);
        }

        grabbing_sellable = false;
        grabbed_sellable = nullptr;
    }
}

void drag_manager::tick()
{
    sf::Mouse mouse;

    bool left = mouse.isButtonPressed(sf::Mouse::Left);

    if(!left)
    {
        if(grab_c == 0)
        {
            grabbing_sellable = false;
            grabbed_sellable = nullptr;

            grabbing_item = false;
            grabbed_item = nullptr;
        }

        grab_c--;
    }

    //printf("shopfront %i\n", shopfront_window_hovered);
}

bool drag_manager::sellable_is_grabbed()
{
    return grabbing_sellable;
}

bool drag_manager::item_is_grabbed()
{
    return grabbing_item;
}

bool drag_manager::any_grabbed()
{
    return sellable_is_grabbed() || item_is_grabbed();
}

bool drag_manager::hovering_over_any_entity()
{
    return entity_num_hovered != -1;
}

bool drag_manager::hovering_over_specific_entity_column()
{
    return entity_column_hovered != -1;
}

bool drag_manager::hovering_over_shopfront_window()
{
    return shopfront_window_hovered;
}

int drag_manager::get_inventory_item_id()
{
    return entity_column_hovered - 5;
}
