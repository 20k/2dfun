#include "drag_manager.hpp"
#include "stats.hpp"
#include "item.hpp"
#include "inventory.hpp"
#include "entities.hpp"
#include "shop.hpp"
#include <imgui/imgui.h>

bool drag_manager::grab_sellable(sellable* s)
{
    if(s == nullptr)
        return false;

    grabbed_sellable = s;
    grabbing_sellable = true;
    grabbed_sellable->locked = true;
    grab_c = 2;

    return true;
}

bool drag_manager::grab_item(item* i)
{
    if(i == nullptr)
        return false;

    //grab_timer = 2;
    grabbed_item = i;
    grabbing_item = true;
    grab_c = 2;

    saved_entity_column = entity_column_hovered;
    saved_entity_num = entity_num_hovered;

    return true;
}

/*bool can_ungrab(drag_manager& drag_manage)
{
    return drag_manage.grab_timer <= 0;
}*/

void drag_manager::tick_entity_grab(entity_manager& entity_manage, shop& s)
{
    if(grabbing_sellable)
    {
        ImGui::SetTooltip(grabbed_sellable->i->display().c_str());
    }

    sf::Mouse mouse;

    bool left = mouse.isButtonPressed(sf::Mouse::Left);
    bool right = mouse.isButtonPressed(sf::Mouse::Right);

    //printf("%i hv\n", (int)hovering_over_any_entity() && sellable_is_grabbed());

    if(!left && hovering_over_any_entity() && sellable_is_grabbed())
    {
        if(entity_num_hovered >= entity_manage.chars.size())
        {
            release_sellable_lock();
            ungrab();

            printf("invalid entity num, in drop path\n");
            return;
        }

        character* c = entity_manage.chars[entity_num_hovered];

        bool success = c->add_to_invent(grabbed_sellable->i);

        if(success)
        {
            s.remove_sellable(grabbed_sellable);
            s.peon_manage.force_unseek(grabbed_sellable);
        }
        else
        {
            release_sellable_lock();
        }

        ungrab();
    }

    if(!left && hovering_over_any_entity() && item_is_grabbed())
    {
        if(entity_num_hovered >= entity_manage.chars.size() || saved_entity_num < 0 || saved_entity_num >= entity_manage.chars.size())
        {
            ungrab();

            printf("invalid entity num, in drop path\n");
            return;
        }

        character* c = entity_manage.chars[entity_num_hovered];
        character* old_c = entity_manage.chars[saved_entity_num];

        if(c != old_c)
        {
            bool success = c->add_to_invent(grabbed_item);

            if(success)
            {
                old_c->remove_from_invent(grabbed_item);
            }
        }

        ungrab();
    }

    if(left && hovering_over_specific_entity_column() && !item_is_grabbed() && !sellable_is_grabbed())
    {
        if(entity_num_hovered >= entity_manage.chars.size())
        {
            ungrab();

            printf("invalid entity num, in specific column grab path\n");
            return;
        }

        character* c = entity_manage.chars[entity_num_hovered];

        int inventory_item = column_id_to_invent(entity_column_hovered);

        item* i = c->invent.get_item(inventory_item);

        ///if i == nullptr, nothing happens
        int s = grab_item(i);

        //printf("%i %i gb\n", s, inventory_item);
    }

    if(!left && hovering_over_shopfront_window() && item_is_grabbed())
    {
        ///uuh. Auto sell?

        if(saved_entity_num >= entity_manage.chars.size() || saved_entity_num < 0)
        {
            ungrab();

            printf("invalid entity num, in specific column drop path\n");
            return;
        }

        character* c = entity_manage.chars[saved_entity_num];

        int inventory_item = column_id_to_invent(saved_entity_column);

        item* i = c->invent.get_item(inventory_item);

        ///we have to nullptr check this as something may have happened in between
        if(i != nullptr)
        {
            s.make_sellable(i);

            entity_manage.fully_remove_item(i);

            ungrab();
        }
    }

    if(item_is_grabbed())
    {
        if(saved_entity_num >= entity_manage.chars.size() || saved_entity_num < 0)
        {
            ungrab();

            printf("invalid entity num, in tooltip\n");
            return;
        }

        character* c = entity_manage.chars[saved_entity_num];

        int inventory_item = column_id_to_invent(saved_entity_column);

        item* i = c->invent.get_item(inventory_item);

        ImGui::SetTooltip(i->display().c_str());

        ImGui::SetWindowPos("Peons", ImVec2(last_entity_window_pos.x(), last_entity_window_pos.y()));

        //printf("D %f %f\n", EXPAND_2(last_entity_window_pos));
    }

    if(sellable_is_grabbed())
    {
        s.peon_manage.force_unseek(grabbed_sellable);
    }

    bool mouse_edge = once<sf::Mouse::Left>(st);

    ///check bounds
    if(mouse_edge && hovering_over_individual_entity_item() && !any_grabbed() && !currently_levelup_clicked)
    {
        if(entity_num_hovered >= entity_manage.chars.size() || entity_num_hovered < 0)
        {
            ungrab();

            printf("invalid entity num, in tooltip\n");
            return;
        }

        character* c = entity_manage.chars[entity_num_hovered];

        int which = entity_individual_to_invent_id(entity_individual_hovered);


        if(which >= 0 && which < stats::stat_names.size())
        {
            currently_levelup_clicked = true;

            mouse_edge = false;
        }
    }

    if(mouse_edge && hovering_over_individual_entity_item() && !any_grabbed() && currently_levelup_clicked)
    {
        currently_levelup_clicked = false;

        mouse_edge = false;
    }

    if(!hovering_over_individual_entity_item())
    {
        currently_levelup_clicked = false;
    }

    if(currently_levelup_clicked)
    {
        ImGui::SetTooltip("Left click again to level");
    }

    if(right)
    {
        currently_levelup_clicked = false;
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
            release_sellable_lock();
            ungrab();
        }

        grab_c--;
    }

    //grab_timer--;

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

bool drag_manager::hovering_over_individual_entity_item()
{
    return entity_individual_hovered != -1;
}

/*int drag_manager::get_inventory_item_id()
{
    return saved_entity_column - 6; ///6 because there are 0 -> 5 before me, and i am 6! 6-6 = 0, aka i'm an idiot
}*/

int drag_manager::column_id_to_invent(int col_id)
{
    return col_id - 6;
}

int drag_manager::entity_individual_to_invent_id(int val)
{
    return val - 6;
}

void drag_manager::ungrab()
{
    grabbing_sellable = false;
    grabbing_item = false;

    grabbed_sellable = nullptr;
    grabbed_item = nullptr;
}

bool drag_manager::carrying_sellable(sellable* s)
{
    if(!grabbing_sellable)
        return false;

    return s == grabbed_sellable;
}

void drag_manager::force_peons_unseek_current_sellable(shop& s)
{
    if(!sellable_is_grabbed())
        return;

    s.peon_manage.force_unseek(grabbed_sellable);
}

void drag_manager::update_entity_window_pos(vec2f p)
{
    last_entity_window_pos = entity_window_pos;
    entity_window_pos = p;
}

void drag_manager::release_sellable_lock()
{
    if(grabbed_sellable != nullptr)
    {
        grabbed_sellable->locked = false;
    }
}
