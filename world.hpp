#ifndef WORLD_HPP_INCLUDED
#define WORLD_HPP_INCLUDED

#include <vector>

struct scenario_manager;
struct draw_manager;

///and so we reach the top
struct world
{
    float time_elapsed_s = 0;

    std::vector<scenario_manager*> scenario_list;

    scenario_manager* make_scenario();

    void destroy_scenario(scenario_manager* s);

    void draw_mission_ui(draw_manager& draw_manage);

    void tick(float dt_s);
};

#endif // WORLD_HPP_INCLUDED
