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

void world::tick(float dt_s)
{
    time_elapsed_s += dt_s;
}
