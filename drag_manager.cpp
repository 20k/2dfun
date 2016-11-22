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
    grabbing = true;
    grab_c = 2;
}

void drag_manager::tick_entity_grab(entity_manager& entity_manage, shop& s)
{
    if(grabbing)
    {
        ImGui::SetTooltip(grabbed_sellable->i->display().c_str());
    }

    sf::Mouse mouse;

    bool left = mouse.isButtonPressed(sf::Mouse::Left);

    if(!left && entity_num_hovered != -1 && grabbing)
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

        grabbing = false;
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
            grabbing = false;
            grabbed_sellable = nullptr;
        }

        grab_c--;
    }
}
