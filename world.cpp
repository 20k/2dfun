#include "world.hpp"
#include "stats.hpp"
#include "item.hpp"
#include "inventory.hpp"
#include "entities.hpp"
#include "economics.hpp"
#include "scenario_constructor.hpp"
#include <imgui/imgui.h>
#include "shop_general_manager.hpp"

scenario_manager* world::make_scenario()
{
    scenario_manager* ns = new scenario_manager;

    scenario_list.push_back(ns);

    return ns;
}

void world::destroy_scenario(scenario_manager* s)
{
    for(int i=0; i<scenario_list.size(); i++)
    {
        if(scenario_list[i] == s)
        {
            scenario_list.erase(scenario_list.begin() + i);

            delete s;

            i--;
        }
    }
}

void world::draw_mission_ui(draw_manager& draw_manage, item_manager* item_manage)
{
    ImGui::Begin("Mission Window");

    for(int i=0; i < scenario_list.size(); i++)
    {
        scenario_manager* scenario_manage = scenario_list[i];

        std::string label = scenarios::types[scenario_manage->type];

        std::string diff = "Diff: " + std::to_string(scenario_manage->difficulty);

        label += "##" + std::to_string(i);

        ImGui::Button(diff.c_str());

        ImGui::SameLine();

        ImGui::Button(label.c_str());

        ImGui::SameLine();

        std::string click_label = "Embark!!!##" + std::to_string(i);

        ///where do we store the items we get back..?
        ///add a vector of unprocessed items?
        ///party? How do we party?
        ///whatever solution we do is going to have to be temporary until we get dragndrop and finalise
        ///how the ui interaction works
        if(ImGui::Button(click_label.c_str()))
        {
            if(party == nullptr)
                continue;

            auto res = embark_mission({party}, scenario_manage, item_manage);

            for(auto& i : res)
            {
                unclaimed_items.push_back(i);
            }
        }
    }

    ImGui::End();
}

void populate_missions(world& w)
{
    ///static std::vector<std::string> scenarios::types
    ///list of mission categories
    ///difficulty from 0 -> 4
    ///stages go from 0 -> ... whatever you want
    ///too long easy essentially means infinite things though
    ///scenario constructor needs to distribute loot first before we write populate_missions

    std::map<int, int> type_num;

    for(auto& i : w.scenario_list)
    {
        type_num[i->type]++;
    }

    for(int i=0; i<scenarios::types.size(); i++)
    {
        if(type_num[i] > 0)
            continue;

        scenario_manager* ns = w.make_scenario();

        ns->init(0, 2, i);
    }
}

void world::tick(float dt_s)
{
    populate_missions(*this);

    time_elapsed_s += dt_s;
}

std::vector<item*> world::embark_mission(const std::vector<entity_manager*>& parties, scenario_manager* s, item_manager* item_manage)
{
    for(entity_manager* entity_manage : parties)
    {
        s->insert_party(*entity_manage);
    }

    s->fully_resolve_scenario();

    auto items = s->distribute_loot(*item_manage);

    return items;
}

std::vector<item*> world::claim_unclaimed_items()
{
    auto it = unclaimed_items;

    unclaimed_items.clear();

    return it;
}

void world::register_as_active_party(entity_manager* _party)
{
    party = _party;
}

void world::populate_shop_entities(const shop_general_manager& shop_general_manage, entity_manager& purchasable_entities)
{
    int num = purchasable_entities.chars.size();

    int num_to_populate = 4;

    for(int i=num; i < num_to_populate; i++)
    {
        character* c = purchasable_entities.make_new(0);

        c->rand_stats();
    }
}
