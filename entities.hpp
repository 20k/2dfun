#ifndef ENTITIES_HPP_INCLUDED
#define ENTITIES_HPP_INCLUDED

#include "shared.hpp"

///need to have bleed damage
struct combat_entity
{
    std::string name;

    float hp = 1.f;
    float hp_max = 1.f;
    int team = 0;

    //virtual void init();

    virtual void register_kill(){};

    virtual float get_dodge_chance(){return 0.f;};
    virtual float get_block_chance(){return 0.f;};

    virtual float calculate_damage(){return 0.f;};

    ///me->calculate_damage() / them->calculate_def_damage_divisor
    virtual float calculate_def_damage_divisor(){return 0.f;};

    ///0 -> miss, 1 -> hit, 2 -> dodge, 3 -> block
    virtual int attack(combat_entity* entity, float my_damage, float& out_damage)
    {
        float dodge_chance = get_dodge_chance();

        if(randf_s(0.f, 1.f) < dodge_chance)
        {
            return 2;
        }

        float their_def = entity->calculate_def_damage_divisor();

        float dam = my_damage / their_def;

        dam *= stats::damage_to_hp_conversion;

        float block_chance = get_block_chance();

        int ret = 1;

        if(randf_s(0.f, 1.f) < block_chance)
        {
            dam *= stats::damage_taken_through_block;

            ret = 3;
        }

        out_damage = dam;

        entity->modify_hp(-dam);

        if(entity->is_dead())
        {
            register_kill();
        }

        return ret;
    }

    virtual ~combat_entity()
    {

    }

    void modify_hp(float hp_change)
    {
        hp += hp_change;

        if(hp > hp_max)
            hp = hp_max;
    }

    void set_team(int _team)
    {
        team = _team;
    }

    virtual bool is_dead(){return false;};
};

///drag and drop weapons onto entity
///we need visual display first
struct character : combat_entity, stattable
{
    inventory invent;

    int cur_level = 1;
    float xp_accum = 0.f;

    std::string classname;
    std::string primary_stat;
    std::string race;

    character()
    {
        init_stats(10.f);
    }

    float get_level(int level)
    {
        float xp = stats::first_level_xp;

        for(int i=1; i<level; i++)
        {
            xp += pow(xp, stats::xp_curve);
        }

        if(level == 0)
            xp = 0;

        return xp;
    }

    ///if this seems incorrect, remember there's an implicit +1!
    float find_level()
    {
        float level = 0;

        for(int i=1; i<100; i++)
        {
            if(get_level(i) > xp_accum)
            {
                level = i;
                break;
            }
        }

        return level;
    }

    float get_accum_xp_relative()
    {
        float level = find_level();

        float upper = get_level(level);

        return upper - get_level(level-1);
    }

    float get_level_adjusted_xp_accum()
    {
        float level = find_level();

        float lower = get_level(level-1);

        return xp_accum - lower;
    }

    float get_raw_level_from_xp()
    {
        return find_level();
    }

    void register_kill() override
    {
        invent.register_kill();
    }

    float get_dodge_chance() override
    {
        float base_dodge = get_item_modified_stat_val("DGE") * stats::dodge_stat_to_percent_dodge  + (get_item_modified_stat_val("DEX") - 10.f) * stats::dex_to_dodge_chance;

        if(primary_stat == "DEX")
        {
            base_dodge *= stats::dex_primary_stat_synergy;
        }

        return base_dodge;
    }

    float get_block_chance() override
    {
        return get_item_modified_stat_val("DEF") * stats::defence_stat_to_percent_block;
    }

    float get_intercept_chance()
    {
        if(primary_stat != "CHA")
            return 0.f;

        return get_item_modified_stat_val("CHA") * stats::cha_intercept_to_percent_block;
    }

    float get_item_modified_stat_val(const std::string& key)
    {
        float val = get_stat_val(key);

        stattable buffs = invent.get_buffs();

        float v2 = buffs.get_stat_val(key);

        return val + v2;
    }

    void recalculate_hp()
    {
        ///old /hp_max = stats::primary_stat_to_hp_mult[primary_stat];

        float new_hp_max = stats::con_to_hp * get_item_modified_stat_val("CON") * stats::primary_stat_to_hp_mult[primary_stat] * stats::class_hp_mult[classname];

        float hp_diff = new_hp_max - hp_max;

        hp += hp_diff;

        hp_max = new_hp_max;
    }

    ///perhaps rand_stats_with_primary, for monsters?
    void rand_stats()
    {
        for(auto& st : stats)
        {
            if(is_secondary(st.key))
                continue;

            vec4i rand_dicerolls = randf<4, int>(1, 7);

            vec4i sorted = rand_dicerolls.sorted();

            int val = sorted.v[1];
            val += sorted.v[2];
            val += sorted.v[3];

            st.val = val;

            //printf("stat %i %s\n", val, st.key.c_str());
        }

        set_stat_val("DEF", 1.f);
        set_stat_val("DGE", 1.f);
        set_stat_val("HEAL", 0.f); ///heal extra. Maybe should be derived stat?

        race = stats::races[randf<1, int>(0, stats::races.size())];
        classname = stats::classnames[randf<1, int>(0, stats::classnames.size())]; ///after this point, flavourtext? Nope. We need real classes
        name = stats::names[randf<1, int>(0, stats::names.size())];

        primary_stat = stats::class_to_damage_stat[classname];

        recalculate_hp();
        hp = hp_max;

        check_forced_classrace();
    }

    void rand_manual_classname(const std::string& crace, const std::string& custom_classname, const std::string& pprimary_stat, int rebal_stat = 0)
    {
        rand_stats();

        race = crace;
        classname = custom_classname;

        primary_stat = pprimary_stat;

        rebalance(primary_stat, rebal_stat);

        recalculate_hp();
        hp = hp_max;

        check_forced_classrace();
    }

    void check_forced_classrace()
    {
        if(classname == "DOG")
            race = "DOG";
    }

    void rebalance(const std::string& stat_into, int num)
    {
        for(int i=0; i<num; i++)
        {
            int rstat = randf<1, int>(0, stats::primary_stat_end);

            if(stats[rstat].key == stat_into)
            {
                i--;
                continue;
            }

            stats[rstat].val -= 1;
        }

        modify_stat_val(stat_into, num);
    }

    float calculate_damage() override
    {
        std::string key = primary_stat;

        float stats_damage_mult = stats::damage_stat_to_damage_mult[key] * stats::class_damage_mult[classname];

        float extra_weapon_scaling_damage = 0.f;

        item* weap = invent.get_weapon();

        if(weap != nullptr)
        {
            std::string wprimary_stat = weap->primary_stat;

            extra_weapon_scaling_damage = invent.get_damage_bonus() * (get_item_modified_stat_val(wprimary_stat) / 10.f);

            //std::cout << "erwep " << std::to_string(extra_weapon_scaling_damage) << std::endl;

            if(weap->primary_stat == primary_stat)
            {
                extra_weapon_scaling_damage *= stats::weapon_stat_synergy_bonus;
            }
        }

        return 1.f * (get_item_modified_stat_val(key) / 10.f) * stats_damage_mult + extra_weapon_scaling_damage;
    }

    float calculate_def_damage_divisor() override
    {
        std::string key = primary_stat;

        float stats_damage_mult = stats::damage_stat_to_damage_mult[key] * stats::class_damage_mult[classname];

        float con_mult = get_item_modified_stat_val("CON") / 10.f;

        float def_stat = stats_damage_mult * con_mult;

        return def_stat;
    }

    std::string display()
    {
        std::string str;

        float total = 0.f;

        str = str + "Name: " + name + "\n";
        str = str + "Race: " + race + "\n";

        if(race != classname)
            str = str + "Class: " + classname + "\n";

        stattable st = invent.get_buffs();

        for(auto& i : stats)
        {
            str = str + i.key + " " + to_string_prec(i.val, 3);// + "\n";

            if(st.get_stat_val(i.key) != 0)
                str = str + " (+" + to_string_prec(st.get_stat_val(i.key), 3) + ")";

            str = str + "\n";

            total += i.val;
        }

        str = str + "Dodge chance: " + to_string_prec(get_dodge_chance() * 100, 4) + "%\n";
        str = str + "Defence chance: " + to_string_prec(get_block_chance() * 100, 4) + "%\n";
        str = str + "Intervene chance: " + to_string_prec(get_intercept_chance() * 100, 4) + "%\n";

        str = str + "Heal per tick: " + to_string_prec(get_teammate_heal(), 3) + "\n";

        str = str + "Total: " + to_string_prec(total, 3) + "\n";

        if(!is_dead())
            str = str + "HP: " + to_string_prec(hp, 3) + "/" + to_string_prec(hp_max, 3) + "\n";
        else
            str = str + "KO (HP max: " + to_string_prec(hp_max, 3) + ")\n";

        if(invent.num() != 0)
        {
            str = str + "Carrying:\n";

            for(auto& i : invent.equipped)
            {
                str = str + i->display();
            }
        }

        return str;
    }

    std::string display_critical()
    {
        std::string str;

         str = str + "Name: " + name + "\n";

         //if(race != classname)
        str = str + "Class: " + classname + "\n";

        str = str + "Heal per tick: " + to_string_prec(get_teammate_heal(), 3) + "\n";

        if(!is_dead())
            str = str + "HP: " + to_string_prec(hp, 3) + "/" + to_string_prec(hp_max, 3) + "\n";
        else
            str = str + "KO (HP max: " + to_string_prec(hp_max, 3) + ")\n";

        if(invent.num() != 0)
        {
            str = str + "Carrying:\n";

            for(auto& i : invent.equipped)
            {
                str = str + i->display();
            }
        }

        str = str + to_string_prec(get_level_adjusted_xp_accum(), 3) + " XP/" + to_string_prec(get_accum_xp_relative(), 3) + "\n";
        //str = str + to_string_prec(xp_accum, 3) + " XP/" + to_string_prec(get_accum_xp_relative(), 3) + "\n";

        return str;
    }

    void level(int level_stat)
    {
        stats[level_stat].val += 1.f;

        recalculate_hp();

        cur_level++;
    }

    void auto_level()
    {
        std::string st;

        st = (randf_s(0.f, 1.f) < 0.5f) ? "CON" : primary_stat;

        level(stat_id(st));
    }

    void delevel_fully()
    {
        for(auto& i : stats)
        {
            if(is_primary(i.key))
            {
                i.val -= 1.f;
            }
        }

        ///not strictly accurate... but what you gunna do
        cur_level-=1.f;
    }

    bool is_dead() override
    {
        return hp <= 0.f;
    }

    float get_difficulty()
    {
        //std::cout << "psfval " + std::to_string(get_stat_val(primary_stat));

        return get_total() * 0.1f + get_item_modified_stat_val(primary_stat) + get_item_modified_stat_val("CON") * 2 * stats::primary_stat_to_hp_mult[primary_stat] * stats::class_hp_mult[classname] + cur_level * 3;
    }

    bool has_weapon()
    {
        return invent.get_weapon() != nullptr;
    }

    inventory& get_invent()
    {
        return invent;
    }

    bool add_to_invent(item* i)
    {
        if(i->is_weapon() && this->has_weapon())
            return false;

        int cnum = invent.get_by_type(i->item_class).size();

        if(cnum >= stats::max_equippable[i->item_class])
            return false;

        invent.add_item(i);

        recalculate_hp();

        return true;
    }

    bool remove_from_invent(item* i)
    {
        invent.remove_item(i);

        recalculate_hp();

        return true;
    }

    float get_teammate_heal()
    {
        float cur = 0.f;

        float wis_heal = get_item_modified_stat_val("WIS") * stats::damage_to_hp_conversion * stats::hpdamage_to_healing_conversion;

        if(primary_stat == "WIS")
            cur = wis_heal;

        float scale_min = 0.5f;
        float scale_max = 1.5f;

        float cur_wissscale = (scale_max - scale_min) * get_item_modified_stat_val("WIS") + scale_min;

        cur += get_item_modified_stat_val("HEAL") * stats::damage_to_hp_conversion * stats::hpdamage_to_healing_conversion * cur_wissscale * stats::heal_stat_heal_mult;// * ((get_item_modified_stat_val("WIS")));

        return cur;
    }

    float get_passive_regen()
    {
        float cur = 0.f;

        cur += get_item_modified_stat_val("HEAL") * stats::damage_to_hp_conversion * stats::hpdamage_to_healing_conversion;

        cur += get_item_modified_stat_val("CON") * stats::damage_to_hp_conversion * stats::hpdamage_to_healing_conversion;

        return cur;
    }

    void do_passive_regen()
    {
        float val = get_passive_regen();

        modify_hp(val);
    }

    ///starts to shine vs 2+ enemies
    float calculate_group_damage()
    {
        if(primary_stat != "INT")
            return 0.f;

        return (get_item_modified_stat_val(primary_stat) / 10.f) * stats::damage_stat_to_damage_mult[primary_stat] * stats::class_damage_mult[classname] + invent.get_damage_bonus() * 0.4f;
    }

    void add_xp(float xp)
    {
        xp_accum += xp;
    }
};

///really each fight should create a new entity manager, and we'll add the correct entities in
struct entity_manager
{
    std::vector<character*> chars;

    int half_turn_counter = 0;

    character* make_new(int team)
    {
        character* c = new character;

        c->set_team(team);

        chars.push_back(c);

        return c;
    }

    void destroy(character* c)
    {
        for(int i=0; i<chars.size(); i++)
        {
            if(chars[i] == c)
            {
                chars.erase(chars.begin() + i);

                delete c;

                i--;
            }
        }
    }

    void insert_character(character* c)
    {
        chars.push_back(c);
    }

    std::vector<character*> get_team(int team)
    {
        std::map<int, std::vector<character*>> team_to_chars;

        for(auto& i : chars)
        {
            team_to_chars[i->team].push_back(i);
        }

        return team_to_chars[team];
    }

    bool all_dead(const std::vector<character*>& team_list)
    {
        for(auto& i : team_list)
        {
            if(!i->is_dead())
                return false;
        }

        return true;
    }

    struct attack_result
    {
        int res;
        character* c1;
        character* c2;
        float real_damage;

        std::vector<character*> savers_of_c2;
        float reduction = 1.f;
    };

    std::string get_battle_message(attack_result r)
    {
        character* c1 = r.c1;
        character* c2 = r.c2;
        int res = r.res;
        float real_damage = r.real_damage;

        std::string ret;

        if(res == 2 || res == 3)
        {
            ret = c2->name + " ";

            if(res == 2)
                ret = ret + "dodges ";
            else
                ret = ret + "blocks ";

            ret = ret + c1->name + "'s attack!";

            if(res == 3)
            {
                ret = ret + " " + to_string_prec(real_damage, 3) + " damage made it through " + c2->name + "'s guard";

                if(c2->is_dead())
                {
                    ret = ret + ", killing them";
                }
            }
        }
        else if(res == 1)
        {
            if(c1->invent.get_weapon_name() == "FISTS")
            {
                ret = c1->name + " smacks " + c2->name;
            }
            else
            {
                ret = c1->name + " strikes " + c2->name + " with their " + c1->invent.get_weapon_name();
            }

            ret = ret + " dealing " + to_string_prec(real_damage, 3) + " damage";

            if(c2->is_dead())
            {
                ret = ret + ", killing them";
            }
        }

        if(r.reduction < 1)
        {
            ret = ret + ". ";

            for(int i=0; i<r.savers_of_c2.size(); i++)
            {
                ret = ret + r.savers_of_c2[i]->name;

                if(i < r.savers_of_c2.size()-1)
                    ret = ret + " and ";
                else
                    ret = ret + " ";
            }

            ret = ret + "intervened to reduce the damage against " + c2->name + " to " + to_string_prec(r.reduction * 100) + "%";

            if(c2->is_dead())
            {
                ret = ret + ", although this was tragically insufficient";
            }
        }

        return ret;
    }

    struct heal_info
    {
        float total_heal = 0.f;

        std::vector<character*> healers;
        std::vector<character*> healed;

        int team = 0;
    };

    /*std::string process_heals(int pteam)
    {
        std::string res;

        std::map<int, std::vector<character*>> team_to_chars;

        for(auto& i : chars)
        {
            team_to_chars[i->team].push_back(i);
        }

        for(auto& team : team_to_chars)
        {
            if(team.first != pteam)
                continue;

            int team_size = team.second.size();

            float heals = 0.f;

            for(auto& ch : team.second)
            {
                if(ch->is_dead())
                    continue;

                heals += ch->get_teammate_heal();

                if(heals > 0)
                    res = res + ch->name + " ";
            }

            heals /= team_size;

            if(heals == 0)
                return res;

            res = res + "heals ";

            for(int i=0; i<team.second.size(); i++)
            {
                auto ch = team.second[i];

                ch->modify_hp(heals);

                res = res + ch->name;

                if(i != team.second.size() - 1)
                {
                    res = res + " and ";
                }
            }

            res = res + " for " + to_string_prec(heals, 3) + " each";

            res = res + "\n";
        }

        return res;
    }*/

    heal_info process_heals(int pteam)
    {
        heal_info info;

        info.team = pteam;

        std::map<int, std::vector<character*>> team_to_chars;

        for(auto& i : chars)
        {
            team_to_chars[i->team].push_back(i);
        }

        for(auto& team : team_to_chars)
        {
            if(team.first != pteam)
                continue;

            int team_size = team.second.size();

            float heals = 0.f;

            for(auto& ch : team.second)
            {
                if(ch->is_dead())
                    continue;

                heals += ch->get_teammate_heal();

                if(ch->get_teammate_heal() > 0)
                    info.healers.push_back(ch);
            }

            heals /= team_size;

            if(heals == 0)
                return info;

            for(int i=0; i<team.second.size(); i++)
            {
                auto ch = team.second[i];

                ch->modify_hp(heals);

                info.healed.push_back(ch);
            }

            info.total_heal = heals;
        }

        return info;
    }

    std::string get_heal_message(heal_info info)
    {
        if(info.healers.size() == 0)
            return "";

        std::string res;

        for(auto& i : info.healers)
        {
            res = res + i->name + " ";
        }

        res = res + "heals ";

        for(int i=0; i<info.healed.size(); i++)
        {
            res = res + info.healed[i]->name;

            if(i != info.healed.size() - 1)
            {
                res = res + " and ";
            }
        }

        res = res + " for " + to_string_prec(info.total_heal, 3) + " each";

        return res;
    }

    std::tuple<float, std::vector<character*>> check_intercept_against(character* c)
    {
        std::map<int, std::vector<character*>> team_to_chars;

        for(auto& i : chars)
        {
            team_to_chars[i->team].push_back(i);
        }

        int cteam = half_turn_counter % 2;
        int oteam = 1 - cteam;

        std::vector<character*> valid_saver_teammates;

        for(int kk=0; kk < team_to_chars[oteam].size(); kk++)
        {
            if(!team_to_chars[oteam][kk]->is_dead())
                valid_saver_teammates.push_back(team_to_chars[oteam][kk]);
        }

        std::vector<character*> saving_teammates;

        float reduction_mult = 1.f;

        for(int i=0; i<valid_saver_teammates.size(); i++)
        {
            if(randf_s(0.f, 1.f) < valid_saver_teammates[i]->get_intercept_chance() && valid_saver_teammates[i] != c)
            {
                reduction_mult *= stats::damage_taken_through_block;

                saving_teammates.push_back(valid_saver_teammates[i]);
            }
        }

        return std::tie(reduction_mult, saving_teammates);
    }

    void attack_single_random(character* c, std::vector<attack_result>& this_tick_results)
    {
        std::map<int, std::vector<character*>> team_to_chars;

        for(auto& i : chars)
        {
            team_to_chars[i->team].push_back(i);
        }

        int cteam = half_turn_counter % 2;
        int other_team = 1 - cteam;

        int other_team_num = team_to_chars[other_team].size();

        std::vector<character*> valid_enemies;

        for(int kk=0; kk < other_team_num; kk++)
        {
            if(!team_to_chars[other_team][kk]->is_dead())
                valid_enemies.push_back(team_to_chars[other_team][kk]);
        }

        if(valid_enemies.size() == 0)
            return;

        int rand_num = randf<1, int>(0, valid_enemies.size());

        character* enemy = valid_enemies[rand_num];


        float reduction_mult;
        std::vector<character*> chars;

        std::tie(reduction_mult, chars) = check_intercept_against(enemy);

        float out_dam = 0.f;

        int res = c->attack(enemy, c->calculate_damage() * reduction_mult, out_dam);

        attack_result result;

        result.res = res;
        result.c1 = c;
        result.c2 = enemy;
        result.reduction = reduction_mult;
        result.savers_of_c2 = chars;
        result.real_damage = out_dam;

        this_tick_results.push_back(result);
    }

    void attack_all(character* c, std::vector<attack_result>& this_tick_results)
    {
        std::map<int, std::vector<character*>> team_to_chars;

        for(auto& i : chars)
        {
            team_to_chars[i->team].push_back(i);
        }

        int cteam = half_turn_counter % 2;
        int other_team = 1 - cteam;

        for(int i=0; i<team_to_chars[other_team].size(); i++)
        {
            character* enemy = team_to_chars[other_team][i];

            float reduction_mult;
            std::vector<character*> chars;

            std::tie(reduction_mult, chars) = check_intercept_against(enemy);

            float out_dam = 0.f;

            int res = c->attack(enemy, c->calculate_group_damage() * reduction_mult, out_dam);

            attack_result result;

            result.res = res;
            result.c1 = c;
            result.c2 = enemy;
            result.reduction = reduction_mult;
            result.savers_of_c2 = chars;
            result.real_damage = out_dam;

            this_tick_results.push_back(result);
        }
    }

    void resolve_half_turn()
    {
        if(fight_over())
        {
            printf("Fight over\n");
            return;
        }

        std::vector<attack_result> this_tick_results;

        std::map<int, std::vector<character*>> team_to_chars;

        for(auto& i : chars)
        {
            team_to_chars[i->team].push_back(i);
        }

        int cteam = half_turn_counter % 2;
        int other_team = 1 - cteam;

        for(int i=0; i<team_to_chars[cteam].size(); i++)
        {
            character* ccharacter = team_to_chars[cteam][i];

            if(ccharacter->is_dead())
                continue;

            ///should i do the intelligent thing here and do the best attack?
            if(ccharacter->calculate_group_damage() == 0)
                attack_single_random(ccharacter, this_tick_results);

            if(ccharacter->calculate_group_damage() > 0)
                attack_all(ccharacter, this_tick_results);
        }

        for(character* i : chars)
        {
            std::cout << i->display() << std::endl;
        }

        for(auto& i : this_tick_results)
        {
            std::cout << get_battle_message(i) << std::endl;
        }

        heal_info healinfo = process_heals(cteam);

        if(healinfo.healers.size() > 0)
            std::cout << get_heal_message(healinfo) << std::endl;

        half_turn_counter++;

        if(fight_over())
        {
            on_fight_end();
        }
    }

    void on_fight_end()
    {
        std::cout << "battle finished" << std::endl;
    }

    bool fight_over()
    {
        return all_dead(get_team(0)) || all_dead(get_team(1));
    }

    std::string display_critical()
    {
        std::string str;

        for(character* i : chars)
        {
            str += i->display_critical() + "\n";
        }

        return str;
    }

    ///one turn of resting
    void idle_turn()
    {
        heal_info healinfo = process_heals(0);

        if(healinfo.healers.size() > 0)
            std::cout << get_heal_message(healinfo) << std::endl;

        for(auto& i : chars)
        {
            i->do_passive_regen();
        }
    }

    ///perhaps its been destroyed, perhaps its been sold
    void fully_remove_item(item* i)
    {
        for(auto& c : chars)
        {
            inventory& invent = c->get_invent();

            invent.remove_item(i);
        }
    }
};


#endif // ENTITIES_HPP_INCLUDED
