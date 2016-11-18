#include "draw_manager.hpp"
#include "stats.hpp"
#include "item.hpp"
#include "inventory.hpp"
#include "entities.hpp"

std::string stringify_to_percent(float val)
{
    return to_string_prec(val * 100, 3);
}

std::string stringify_natural(float val)
{
    return to_string_prec(val * 100, 3);
}

void draw_manager::draw_entity_ui(entity_manager& entity_manage)
{
    if(entity_manage.chars.size() == 0)
        return;

    character* c = entity_manage.chars[3];

    //Button(const char* label, const ImVec2& size = ImVec2(0,0));
    float button_width = 50.f;
    float half_sep = 15;

    float full_width = button_width*2 + half_sep;

    ImGui::Begin("Peon");

    ImGui::Button(c->name.c_str(), ImVec2(full_width, 0));
    ImGui::Button(c->classname.c_str(), ImVec2(full_width, 0));

    if(c->classname != c->race.c_str())
        ImGui::Button(c->race.c_str(), ImVec2(full_width, 0));

    int clevel = c->cur_level;
    int potential_level = c->get_raw_level_from_xp();

    std::string rlevel = to_string_prec(clevel, 3);
    std::string extra_levels = to_string_prec(potential_level - clevel);

    std::string level_string = rlevel;

    if(potential_level - clevel > 0)
    {
        level_string += " (+" + extra_levels + ")";
    }

    ImGui::Button(("LEVEL: " + level_string).c_str(), ImVec2(full_width, 0));

    std::string hp_str = "HP: " + to_string_prec(c->hp, 3) + "/" + to_string_prec(c->hp_max, 3);

    if(c->hp < 0)
    {
        hp_str = "KO'd " + std::string("(") + to_string_prec(c->hp_max, 3) + ")";
    }

    ImGui::Button(hp_str.c_str(), ImVec2(full_width, 0));

    ImGui::Columns(2);

    ImGui::SetColumnOffset(1, button_width + half_sep);

    //ImGui::Button("LVL", ImVec2(button_width, 0));

    for(auto& i : c->stats)
    {
        ImGui::Button(i.key.c_str(), ImVec2(button_width, 0));
    }

    ImGui::NextColumn();

    //ImGui::Button(level_string.c_str(), ImVec2(button_width, 0));

    stattable buffs = c->invent.get_buffs();

    for(auto& i : c->stats)
    {
        std::string val = to_string_prec(i.val, 3);

        float v2 = buffs.get_stat_val(i.key);

        if(v2 > 0)
            val = val + " (+" + to_string_prec(v2, 3) + ")";

        ImGui::Button(val.c_str(), ImVec2(button_width, 0));
    }

    ImGui::Columns(1);

    //std::string blk_chance = to_string_prec(c->get_block_chance()*100, 3);

    ///def dodge heal
    ImGui::Text("Block Chance %%%s", stringify_to_percent(c->get_block_chance()).c_str());

    //std::string dge_chance = to_string_prec(c->get_dodge_chance()*100, 3);

    ///def dodge heal
    ImGui::Text("Dodge Chance %%%s", stringify_to_percent(c->get_dodge_chance()).c_str());

    ImGui::Text("Intervene Chance %%%s", stringify_to_percent(c->get_intercept_chance()).c_str());

    ImGui::Text("Group Heal Per Turn %s", stringify_to_percent(c->get_teammate_heal()).c_str());

    std::string xp_str = "XP " + to_string_prec(c->get_level_adjusted_xp_accum(), 3) + "/" + to_string_prec(c->get_accum_xp_relative(), 3);

    ImGui::Text("%s", xp_str.c_str());

    ImGui::End();

}
