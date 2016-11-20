#ifndef ITEM_HPP_INCLUDED
#define ITEM_HPP_INCLUDED

#include "shared.hpp"

struct item : stattable
{
    int id = -1;

    int accumulated_wear = 0;
    int maintenance = 0;
    int notoriety = 0;
    int kills = 0;

    ///times base_stat
    float attack_boost_hp_flat = 0.f;

    ///(character stat + stat_boost)/10 * attack_boost_hp_flat
    std::string primary_stat;
    std::string item_class;
    std::string description;
    int weapon_class = -1;
    int rarity = 0;

    void set_attack_boost(float hp)
    {
        attack_boost_hp_flat = hp;
    }

    item(int _id)
    {
        init_stats(0.f);
        id = _id;
    }

    void init_weapon_class(int id, float extra_hp_damage)
    {
        if(id >= stats::weapon_class.size())
        {
            printf("what in init_weapon_class %i\n", id);
            return;
        }

        weapon_class = id;

        primary_stat = stats::weapon_class_to_primary_stat[weapon_class];
        item_class = stats::weapon_class[weapon_class];

        set_attack_boost(extra_hp_damage);
    }

    void init_stat_boosts(const std::vector<base_stat>& extra_stats)
    {
        for(auto& s : extra_stats)
        {
            modify_stat_val(s.key, s.val);
        }
    }

    void init_item(int id)
    {
        if(id >= stats::item_class.size())
        {
            printf("what in init_item %i\n", id);
            return;
        }

        item_class = stats::item_class[id];
    }

    void set_description(const std::string& desc)
    {
        description = desc;
    }

    std::string display()
    {
        std::string ret;

        std::string stat_string = stat_display();

        int capped_rarity = std::min(rarity, (int)stats::item_rarity.size()-1);

        ret = stats::item_rarity[capped_rarity] + " " + item_class;

        if(stat_string.size() > 0)
            ret = ret + " of " + stat_display() + "\n";// + description;

        std::string first_letter = stat_string.size() > 0 ? "D" : " d";

        if(weapon_class != -1)
        {
            ret = ret + first_letter + "ealing an extra " + to_string_prec(attack_boost_hp_flat, 3) + " hp of damage\n";
        }
        else
        {
            ret = ret + "\n";
        }

        ret = ret + "Condition: " + stats::weapon_wear[get_wear()] + "\n";
        ret = ret + "Notoriety: " + stats::weapon_notoriety[get_notoriety()] + "\n";

        ret = ret + description;

        if(ret.back() != '\n')
            ret.push_back('\n');

        return ret;
    }

    void random_item_or_weapon()
    {
        if(randf_s(0.f, 1.f) < 0.5f)
        {
            int rv = randf<1, int>(0, stats::weapon_class.size());

            random_weapon_with_class(rv);
        }
        else
        {
            int rv = randf<1, int>(0, stats::item_class.size());

            init_item(rv);
        }
    }

    void random_magical(int extra_stats)
    {
        std::vector<int> ids;

        for(int i=0; i<extra_stats; i++)
        {
            ids.push_back(randf<1, int>(0, stats.size()));
        }

        for(auto& i : ids)
        {
            stats[i].val += 1;
        }

        rarity += extra_stats;
    }

    void random_weapon_with_class(int wclass)
    {
        float damage = pow(randf_s(0.f, 1.f), stats::weapon_find_power) * stats::weapon_damage_max;

        init_weapon_class(wclass, damage);
    }

    void random_stat_appropriate_weapon(const std::string& stat)
    {
        int id = stat_id(stat);

        std::vector<int> appropriate_weapons;

        for(auto& i : stats::weapon_class_to_primary_stat)
        {
            if(i.second == stat)
            {
                appropriate_weapons.push_back(i.first);
            }
        }

        if(appropriate_weapons.size() == 0)
        {
            std::cout << "no weapons for stat " << stat << std::endl;
            return;
        }

        int random_weapon_num = randf<1, int>(0, appropriate_weapons.size());

        int random_weapon = appropriate_weapons[random_weapon_num];

        random_weapon_with_class(random_weapon);
    }

    void inc_stat(const std::string& key, int val = 1)
    {
        modify_stat_val(key, val);
    }

    void inc_accumulated_wear()
    {
        accumulated_wear++;
    }

    void inc_notoriety()
    {
        notoriety++;
    }

    void inc_maintenance()
    {
        maintenance++;
    }

    int get_wear()
    {
        //return std::max(0, outings - maintenance);

        return clamp(accumulated_wear - maintenance, 0, stats::weapon_wear.size());
    }

    int get_notoriety()
    {
        return clamp(notoriety, 0, stats::weapon_notoriety.size());
    }

    void register_kill()
    {
        kills++;

        if(get_wear() == 0)
        {
            inc_accumulated_wear();
        }
    }

    int get_kills()
    {
        return kills;
    }
};

struct item_manager
{
    std::vector<item*> items;

    int gid = 0;

    item* make_new()
    {
        return new item(gid++);
    }

    void destroy(item* f)
    {
        for(int i=0; i<items.size(); i++)
        {
            if(items[i] == f)
            {
                items.erase(items.begin() + i);
                delete f;

                i--;
            }
        }
    }
};

#endif // ITEM_HPP_INCLUDED
