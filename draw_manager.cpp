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

    ImGui::Begin("Paryay");

    ImGui::Button(c->name.c_str(), ImVec2(full_width, 0));

    ImGui::Columns(2);

    ImGui::SetColumnOffset(1, button_width + half_sep);

    for(auto& i : c->stats)
    {
        ImGui::Button(i.key.c_str(), ImVec2(button_width, 0));
    }

    ImGui::NextColumn();

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



    ImGui::End();

}
