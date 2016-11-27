#include "world.hpp"
#include "stats.hpp"
#include "item.hpp"
#include "inventory.hpp"
#include "entities.hpp"
#include "economics.hpp"
#include "scenario_constructor.hpp"

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

void world::draw_mission_ui(draw_manager& draw_manage)
{

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

void world::embark_mission(std::vector<entity_manager*>& parties, scenario_manager* s)
{
    for(entity_manager* entity_manage : parties)
    {
        s->insert_party(*entity_manage);
    }
}
