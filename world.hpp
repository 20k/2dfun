#ifndef WORLD_HPP_INCLUDED
#define WORLD_HPP_INCLUDED

#include <vector>

struct scenario_manager;
struct draw_manager;
struct entity_manager;
struct item;
struct item_manager;

///and so we reach the top
struct world
{
    float time_elapsed_s = 0;

    entity_manager* party = nullptr;

    std::vector<scenario_manager*> scenario_list;

    std::vector<item*> unclaimed_items;

    scenario_manager* make_scenario();

    void destroy_scenario(scenario_manager* s);

    void draw_mission_ui(draw_manager& draw_manage, item_manager* item_manage);

    void tick(float dt_s);

    std::vector<item*> embark_mission(const std::vector<entity_manager*>& parties, scenario_manager* s, item_manager* item_manage);

    std::vector<item*> claim_unclaimed_items();

    void register_as_active_party(entity_manager* _party);
};

#endif // WORLD_HPP_INCLUDED
