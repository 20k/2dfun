#ifndef INVENTORY_HPP_INCLUDED
#define INVENTORY_HPP_INCLUDED

#include "shared.hpp"

///sort invent
struct inventory
{
    ///equipped is anything you take with you on a quest, cant use loot during?
    ///Ok. Loot probably generated completely separately, not a carried or inventory at all
    std::vector<item*> equipped;

    ///only taking one weapon is going to have to be ui enforced? We cant ditch them here...
    ///I guess we'll need to enforce armour, and ring constraints etc as well
    stattable get_buffs()
    {
        stattable st;
        st.init_stats(0.f);

        for(item* i : equipped)
        {
            st.add(i->stats);
        }

        return st;
    }

    ///must be 0 or 1
    int get_weapon_num()
    {
        int n = 0;

        for(item* i : equipped)
        {
            if(i->weapon_class != -1)
                n++;
        }

        return n;
    }

    void add_item(item* i)
    {
        equipped.push_back(i);
    }

    int num()
    {
        return equipped.size();
    }

    float get_damage_bonus()
    {
        float bonus = 0.f;

        for(auto& i : equipped)
        {
            if(i->attack_boost_hp_flat != 0)
            {
                bonus += i->attack_boost_hp_flat;
            }
        }

        return bonus;
    }

    float get_weapon_primary_stat_value()
    {
        item* i = get_weapon();

        if(i == nullptr)
            return 0.f;

        return i->get_stat_val(i->primary_stat);
    }

    ///only 1!
    item* get_weapon()
    {
        for(item* i : equipped)
        {
            if(i->weapon_class != -1)
                return i;
        }

        return nullptr;
    }

    std::string get_weapon_name()
    {
        item* i = get_weapon();

        if(i == nullptr)
        {
            return "FISTS";
        }

        return i->item_class;
    }

    void register_kill()
    {
        item* ptr = get_weapon();

        if(ptr != nullptr)
        {
            ptr->register_kill();
        }
    }

    std::vector<item*> get_by_type(const std::string& item_classname)
    {
        std::vector<item*> ret;

        for(item* i : equipped)
        {
            if(i->item_class == item_classname)
            {
                ret.push_back(i);
            }
        }

        return ret;
    }
};

#endif // INVENTORY_HPP_INCLUDED
