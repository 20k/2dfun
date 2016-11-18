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
        10000,
        100000
    };

    ///value for damage scaling
    static float damage_max_value = 10000.f;

    static float percent_value_lost_per_wear_level = 0.1;

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

        rarity = i->rarity;

        rarity = std::min(rarity, (int)economics::approx_economic_value.size()-1);

        nominal_value = economics::approx_economic_value[rarity] + randf_s(-0.2, 0.2f) * economics::approx_economic_value[rarity];

        value_added_damage = i->attack_boost_hp_flat * economics::damage_max_value;

        wear = i->get_wear();

        notoriety = i->notoriety;

        float fnot = (float)notoriety / stats::weapon_notoriety.size();

        value = nominal_value + value_added_damage;

        value = value - value * wear * economics::percent_value_lost_per_wear_level + value * fnot * economics::notoriety_value_mult_max;
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
