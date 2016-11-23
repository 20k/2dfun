#ifndef ECONOMICS_HPP_INCLUDED
#define ECONOMICS_HPP_INCLUDED

#include "shared.hpp"

namespace economics
{
    /**"
         Common",
        "Good Quality",
        "Uncommon",
        "Rare",
        "Ultra rare",
        "Holy crap!"*/
    ///see rarity
    static std::vector<int> approx_economic_value
    {
        100,
        200,
        700,
        2000,
        5000,
        10000
    };

    ///value for damage scaling
    //static float damage_max_value = 2000.f;

    static float damage_percent_add_value_max = 0.1f;

    static float percent_value_lost_per_wear_level = 0.2;

    static float notoriety_value_mult_max = 10;
}

///Ooh. We need a loot table, of course!
///First, a scenario constructor?
struct economic_item
{
    float value = 0.f;

    int rarity = 0;
    int nominal_value = 0;
    int value_added_damage = 0;
    int wear = 0;
    int notoriety = 0;

    void load_from_item(item* i)
    {
        srand(i->id);

        for(int kk=0; kk<20; kk++)
            rand();

        rarity = i->rarity;

        rarity = std::min(rarity, (int)economics::approx_economic_value.size()-1);

        nominal_value = economics::approx_economic_value[rarity] + randf_s(-0.005, 0.005f) * economics::approx_economic_value[rarity];

        float weapon_frac = i->attack_boost_hp_flat / stats::weapon_damage_max;

        float random_extra_damage_frac = i->extra_damage / stats::weapon_damage_random;

        //value_added_damage = weapon_frac * economics::damage_max_value;

        //value_added_damage = i->attack_boost_hp_flat * economics::damage_max_value;

        //value_added_damage = pow(weapon_frac, stats::weapon_find_power/1.3f) * economics::damage_max_value;
        //value_added_damage = pow(i->attack_boost_hp_flat / stats::weapon_damage_max, 1.f) * economics::damage_max_value;

        //value_added_damage = ((1.f - normal_cdf(weapon_frac)) - 0.5f) * economics::damage_max_value;

        //weapon_frac = clamp(weapon_frac, 0.f,1.f);

        //float cumulative_probability = (normal_cdf(weapon_frac) - 0.5f) * 2
        //value_added_damage = cumulative_probability * economics::damage_max_value;

        wear = i->get_wear();

        notoriety = i->notoriety;

        float fnot = (float)notoriety / stats::weapon_notoriety.size();

        value = nominal_value;

        value_added_damage = value * random_extra_damage_frac * economics::damage_percent_add_value_max;

        value = value
         - value * wear * economics::percent_value_lost_per_wear_level + value * fnot * economics::notoriety_value_mult_max
         + value_added_damage;

        value = (int)value;
    }

    std::string display()
    {
        std::string ret;

        ret += "Value: " + to_string_prec(value, 3) + "\n";

        ret += "Nominal value: " + to_string_prec(nominal_value, 3) + "\n";

        ret += "Rarity: " + stats::item_rarity[rarity] + "\n";

        ret += "Wear: " + stats::weapon_wear[wear];

        if(value_added_damage > 0)
        {
            ret += "Extra value added due to increased damage: " + to_string_prec(value_added_damage, 3);
        }

        if(notoriety > 0)
            ret += "Notoriety: " + stats::weapon_notoriety[notoriety] + "\n";

        return ret;
    }
};

#endif // ECONOMICS_HPP_INCLUDED
