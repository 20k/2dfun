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

std::string fix_imgui_percent(std::string str)
{
    for(int i=1; i<str.size(); i++)
    {
        if(str[i-1] == '%' && str[i] != '%')
        {
            str.insert(str.begin() + i-1, '%');
            i++;
        }
    }

    return str;
}

int get_len_nodoublecount_percents(std::string str)
{
    int len = str.size();

    for(int i=1; i<str.size(); i++)
    {
        if(str[i-1] == '%' && str[i] == '%')
        {
            len--;
        }
    }

    return len;
}

void render_character_ui(character* c, int column_id)
{
    //Button(const char* label, const ImVec2& size = ImVec2(0,0));
    float button_width = 50.f;
    float half_sep = 4;

    ///Frame padding
    float imgui_padding_thing = 4;

    float pad = half_sep / 2.f - imgui_padding_thing;

    float full_width = button_width*2 + half_sep;

    ImGui::BeginGroup();

    ImGui::Button(c->name.c_str(), ImVec2(full_width, 0));
    ImGui::Button(c->classname.c_str(), ImVec2(full_width, 0));

    //if(c->classname != c->race.c_str())
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

    ImGui::Button(("LVL: " + level_string).c_str(), ImVec2(full_width, 0));

    std::string hp_str = "HP: " + to_string_prec(c->hp, 3) + "/" + to_string_prec(c->hp_max, 3);

    if(c->hp < 0)
    {
        hp_str = "KO'd " + std::string("(") + to_string_prec(c->hp_max, 3) + ")";
    }

    ImGui::Button(hp_str.c_str(), ImVec2(full_width, 0));

    #define NO_COLUMNS
    #ifndef NO_COLUMNS
    ImGui::Columns(2);

    ImGui::SetColumnOffset(1, (button_width + half_sep) + (button_width + half_sep + half_sep) * column_id * 2);

    //ImGui::Button("LVL", ImVec2(button_width, 0));

    ///defence dodge and heal are possible too much info
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
    #endif

    #ifdef NO_COLUMNS
    stattable buffs = c->invent.get_buffs();

    for(auto& i : c->stats)
    {
        std::string val = to_string_prec(i.val, 3);

        float v2 = buffs.get_stat_val(i.key);

        if(v2 > 0)
            val = val + " (+" + to_string_prec(v2, 3) + ")";

        ImGui::Button(i.key.c_str(), ImVec2(button_width + pad, 0));

        //ImGui::SameLine(0.f, half_sep);
        ImGui::SameLine();

        ImGui::Button(val.c_str(), ImVec2(button_width + pad, 0));
    }
    #endif

    //std::string blk_chance = to_string_prec(c->get_block_chance()*100, 3);

    ///def dodge heal
    ImGui::Text("Block: %s%%", stringify_to_percent(c->get_block_chance()).c_str());

    //std::string dge_chance = to_string_prec(c->get_dodge_chance()*100, 3);

    ///def dodge heal
    ImGui::Text("Dodge: %s%%", stringify_to_percent(c->get_dodge_chance()).c_str());

    ImGui::Text("Intervene: %s%%", stringify_to_percent(c->get_intercept_chance()).c_str());

    ImGui::Text("Heal per tick %s", stringify_to_percent(c->get_teammate_heal()).c_str());

    std::string xp_str = "XP " + to_string_prec(c->get_level_adjusted_xp_accum(), 3) + "/" + to_string_prec(c->get_accum_xp_relative(), 3);

    ImGui::Text("%s", xp_str.c_str());

    ImGui::EndGroup();

    //printf("%f\n", ImGui::CalcItemWidth());
}

void render_character_alt(character* c, int column_id)
{
    float button_width = 80.f;

    std::vector<std::string> displays;

    std::string name = c->name;
    std::string classname = c->classname;
    std::string race = c->race;

    int clevel = c->cur_level;
    int potential_level = c->get_raw_level_from_xp();

    std::string rlevel = to_string_prec(clevel, 3);
    std::string extra_levels = to_string_prec(potential_level - clevel);

    std::string level_string = rlevel;

    if(potential_level - clevel > 0)
    {
        level_string += " (+" + extra_levels + ")";
    }

    level_string = "LVL: " + level_string;

    std::string hp_str = "HP: " + to_string_prec(c->hp, 3) + "/" + to_string_prec(c->hp_max, 3);

    if(c->hp < 0)
    {
        hp_str = "KO'd " + std::string("(") + to_string_prec(c->hp_max, 3) + ")";
    }

    std::string xp_str = "XP " + to_string_prec(c->get_level_adjusted_xp_accum(), 3) + "/" + to_string_prec(c->get_accum_xp_relative(), 3);

    displays.push_back(name);
    displays.push_back(classname);
    displays.push_back(race);

    displays.push_back(level_string);
    displays.push_back(hp_str);
    displays.push_back(xp_str);

    stattable buffs = c->invent.get_buffs();

    for(auto& i : c->stats)
    {
        std::string val = to_string_prec(i.val, 3);

        float v2 = buffs.get_stat_val(i.key);

        if(v2 > 0)
            val = val + " (+" + to_string_prec(v2, 3) + ")";

        std::string res = i.key + " " + val;

        displays.push_back(res);
    }

    float r12_size = button_width * 2;

    int num_in_row = ceilf(displays.size() / 3.f);

    ImGui::BeginGroup();

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<num_in_row; j++)
        {
            int id = j * 3 + i;

            if(id >= displays.size())
                continue;

            if(j != 0)
                ImGui::SameLine();

            float csize = button_width;

            if(j == 0 || j == 1)
                csize = r12_size;

            ImGui::Button(displays[id].c_str(), ImVec2(csize, 0.f));
        }

        if(i == 2)
        ImGui::NewLine();
    }

    ImGui::EndGroup();
}

struct render_info
{
    std::string tooltip;
    std::string str;
    vec3f col = {1,1,1};

    render_info(const std::string& s)
    {
        str = s;
    }

    render_info(const std::string& s, vec3f c, const std::string& t = "")
    {
        str = s;
        col = c;
        tooltip = t;
    }
};

std::vector<render_info> get_render_strings(character* c)
{
    std::vector<render_info> displays;

    std::string name = c->name;
    std::string classname = c->classname;
    std::string race = c->race;

    int clevel = c->cur_level;
    int potential_level = c->get_raw_level_from_xp();

    std::string rlevel = to_string_prec(clevel, 3);
    std::string extra_levels = to_string_prec(potential_level - clevel);

    std::string level_string = rlevel;

    if(potential_level - clevel > 0)
    {
        level_string += " (+" + extra_levels + ")";
    }

    level_string = "LVL: " + level_string;

    std::string hp_str = "HP: " + to_string_prec(c->hp, 3) + "/" + to_string_prec(c->hp_max, 3);

    if(c->hp < 0)
    {
        hp_str = "KO'd " + std::string("(") + to_string_prec(c->hp_max, 3) + ")";
    }

    std::string xp_str = "XP " + to_string_prec(c->get_level_adjusted_xp_accum(), 3) + "/" + to_string_prec(c->get_accum_xp_relative(), 3);


    displays.push_back(name);
    displays.push_back(classname);
    displays.push_back(race);

    displays.push_back(level_string);
    displays.push_back(hp_str);
    displays.push_back(xp_str);

    ///we need to know what item is given which buffs
    stattable buffs = c->invent.get_buffs();

    int max_character_length = 0;

    for(auto& i : c->stats)
    {
        std::string val = to_string_prec(i.val, 3);

        float v2 = buffs.get_stat_val(i.key);

        #ifdef USE_PLUSBRACKET_NOTATION

        if(v2 > 0)
            val = val + " (+" + to_string_prec(v2, 3) + ")";

        #endif

        int iv = clamp(v2, 0, stats::colour_table.size()-1);

        #define USE_COLOURS
        #ifdef USE_COLOURS

        vec3f col = {1,1,1};

        if(v2 > 0)
            col = stats::colour_table[iv];

        #endif

        #ifndef USE_PLUSBRACKET_NOTATION
        float combo = i.val + v2;

        val = to_string_prec(combo);
        #endif // USE_PLUSBRACKET_NOTATION

        std::string tooltip_optional;

        #define USE_TOOLTIP
        #ifdef USE_TOOLTIP

        if(v2 > 0)
            tooltip_optional = "Base " + to_string_prec(i.val, 3) + " with +" + to_string_prec(v2, 3) + " from item(s)";

        #endif // USE_TOOLTIP

        std::string res = i.key + " " + val;

        displays.push_back(render_info(res, col, tooltip_optional));
    }

    ///do fun with wepons here

    inventory& invent = c->invent;


    /*Uncommon BOW of +1 STR +1 CHA
    Dealing an extra 0.135 hp of damage
    Condition: PRISTINE
    Notoriety: GOOD*/

    if(invent.equipped.size() != 0)
    {
        std::string pad = ":";

        displays.push_back(pad + "E:");
        displays.push_back(pad + "Q:");
        displays.push_back(pad + "P:");
    }

    ///condition and notoriety as tooltips
    for(int i=0; i<invent.equipped.size(); i++)
    {
        item* it = invent.equipped[i];

        std::string kv_stats;

        int rarity = clamp(it->rarity, 0, stats::item_rarity.size()-1);

        if(rarity > 0)
        {
            auto base_stats = it->stats;

            for(auto& i : base_stats)
            {
                if(i.val <= 0)
                    continue;

                kv_stats += i.key + " +" + to_string_prec(i.val, 3) + "\n";
            }
        }

        vec3f rarity_col = stats::colour_table[rarity];

        ///construct row by rows

        std::string str = it->item_class;

        if(it->weapon_class != -1)
            str += " (" + it->primary_stat + ")";

        render_info name_info(str, rarity_col, kv_stats);

        displays.push_back(name_info);

        std::string rarity_str = stats::item_rarity[rarity];

        render_info rinfo(rarity_str);//, rarity_col, kv_stats);

        displays.push_back(rinfo);

        float hp_damage = it->attack_boost_hp_flat;

        std::string hp_str = "Dam: " + to_string_prec(hp_damage, 3);

        displays.push_back(hp_str);
    }

    for(auto& i : displays)
    {
        i.str = fix_imgui_percent(i.str);
    }

    return displays;
}

std::vector<int> get_max_in_3_group(character* c)
{
    std::vector<int> max_in_3_group;

    std::vector<render_info> displays = get_render_strings(c);

    for(int i=0; i<displays.size(); i++)
    {
        int column_num = i / 3;

        if(max_in_3_group.size() < column_num+1)
            max_in_3_group.push_back(0);

        int str_len = get_len_nodoublecount_percents(displays[i].str);

        max_in_3_group[column_num] = std::max(max_in_3_group[column_num], str_len);
    }

    return max_in_3_group;
}

std::vector<int> combine_3_group(const std::vector<int>& r1, const std::vector<int>& r2)
{
    assert(r1.size() == r2.size());

    std::vector<int> ret;
    ret.resize(r2.size());

    for(int i=0; i<r2.size(); i++)
    {
        ret[i] = std::max(r1[i], r2[i]);
    }

    return ret;
}

void render_character_text(character* c, int column_id, const std::vector<int>& max_in_3_group)
{
    //std::vector<int> max_in_3_group;
    float button_width = 80.f;

    std::vector<render_info> displays = get_render_strings(c);

    //for(auto& i : displays)
    /*for(int i=0; i<displays.size(); i++)
    {
        int column_num = i / 3;

        if(max_in_3_group.size() < column_num+1)
            max_in_3_group.push_back(0);

        int str_len = get_len_nodoublecount_percents(displays[i]);

        max_in_3_group[column_num] = std::max(max_in_3_group[column_num], str_len);
    }*/

    //max_in_3_group = get_max_in_3_group(c);

    float r12_size = button_width * 2;

    int num_in_row = ceilf(displays.size() / 3.f);

    ImGui::BeginGroup();

    std::string res;

    int extra_spacing = 2;

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<num_in_row; j++)
        {
            int id = j * 3 + i;

            if(id >= displays.size())
                continue;

            std::string cur = displays[id].str;
            vec3f col = displays[id].col;

            int cmax = max_in_3_group[j];

            int str_len = get_len_nodoublecount_percents(cur);

            for(int i=str_len; i<cmax + extra_spacing; i++)
            {
                cur = cur + " ";
            }

            res = res + cur + " ";

            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(col.v[0], col.v[1], col.v[2], 1));

            ImGui::Text((cur + " ").c_str());

            ImGui::PopStyleColor();

            if(ImGui::IsItemHovered())
            {
                std::string tooltip = displays[id].tooltip;

                if(tooltip != "")
                {
                    ImGui::SetTooltip(tooltip.c_str());
                }
            }

            ImGui::SameLine(0, 0);
        }

        ImGui::NewLine();

        res = res + "\n";
    }

    //ImGui::Text(res.c_str());

    ImGui::EndGroup();
}

void render_test(character* c)
{
    ImGui::BeginGroup();
    ImGui::Text("Text 1");
    ImGui::Text("Text 2");

    //ImGui::Columns(2);
    //ImGui::Columns(1);

    ImGui::EndGroup();

    /*ImGui::NextColumn();-

    ImGui::BeginGroup();
    ImGui::Text("Text 3");
    ImGui::Text("Text 4");
    ImGui::EndGroup();*/
}

void draw_manager::draw_entity_ui(entity_manager& entity_manage)
{
    if(entity_manage.chars.size() == 0)
        return;

    //character* c = entity_manage.chars[3];
    ImGui::Begin("Peons");

    float horizontal_sep = 20.f;

    //ImGui::Columns(entity_manage.chars.size());

    //std::vector<std::vector<render_info>> displays;

    std::vector<int> max_in_3_group;

    for(auto& i : entity_manage.chars)
    {
        //displays.push_back(get_render_strings(i));

        auto tmax = get_max_in_3_group(i);

        ///will alloc first time, do nothing the rest of the time
        max_in_3_group.resize(tmax.size());

        max_in_3_group = combine_3_group(max_in_3_group, tmax);
    }

    for(int i=0; i<entity_manage.chars.size(); i++)
    {
        character* c = entity_manage.chars[i];

        render_character_text(c, i, max_in_3_group);

        //render_test(c);

        //if(i == entity_manage.chars.size()-1)
            //ImGui::NextColumn();

        //ImGui::SameLine(0.f, horizontal_sep);

        ImGui::Separator();
    }

    //ImGui::Columns(1);

    ImGui::End();
}
