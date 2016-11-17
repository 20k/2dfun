#ifndef STATS_HPP_INCLUDED
#define STATS_HPP_INCLUDED

#include "shared.hpp"

struct base_stat
{
    std::string key;
    float val = 10.f;
};

///tomorrow is loot and economy, cha and luck. Luck invisible?
namespace stats
{
    ///add defence?
    std::vector<std::string> stat_names =
    {
        "STR",
        "CON",
        "DEX",
        "INT",
        "WIS",
        "CHA"
    };

    int primary_stat_end = stat_names.size();

    std::vector<std::string> secondary_stats =
    {
        "DEF",
        "DGE", ///not a %, a stat
        "HEAL",
    };

    std::vector<int> can_be_a_magical_property =
    {
        1, ///str
        1, ///con
        1, ///dex
        1, ///int
        1, ///wis
        1, ///cha
        1, ///def
        1  ///dodge stat
    };

    std::vector<base_stat> default_stats(float to_what)
    {
        std::vector<base_stat> ret;

        for(auto& i : stat_names)
        {
            ret.push_back({i, to_what});
        }

        for(auto& i : secondary_stats)
            ret.push_back({i, to_what});

        return ret;
    }

    /*
    std::vector<base_stat> default_secondary_stats(float to_what = 0.f)
    {
        std::vector<base_stat> ret;

        for(auto& i : secondary_stats)
        {
            ret.push_back({i, 0.f});
        }

        return ret;
    }*/

    int num = stat_names.size();

    std::vector<std::string> races =
    {
        "HUMIE",
        "HUMUN",
        "HUGH MANN"
        "ORC",
        "ORK",
        "ORCK",
        "LOBSTER",
        "LIZARD",
        "TROLL",
        "SHELLFISH",
        "WEARWOLF",
        "DARGON",
        "AWAREWOLF",
        "BEAR",
        "HALF-BEAR, ALL-MAN",
        "BEARINGTON",
        "LEPER",
        "LEPERD",
        "QUARTERLING",
        "100% PEAR"
    };

    std::vector<std::string> names =
    {
        "JIM",
        "BOB",
        "PETE",
        "LIZZY",
        "IZZY",
        "SHARON",
        "GERMAINE",
        "INSERT NAME HERE",
        "N/A",
        "PHIL",
        "ABLE",
        "BABLE",
        "SEEBLE",
        "DWEEBLE",
        "EVIL",
        "ETHIL",
        "GERBIL",
        "HANDLE",
        "IGOR",
        "JANDLE",
        "KRUNDLE",
        "LADLE",
        "MANDY",
        "NANDY",
        "OPRY",
        "PROUDLOBE",
        "Q",
        "RANDAL",
        "SANDY",
        "TAMMY",
        "UVULA",
        "VUVUZULA",
        "WUV-U-YEW-SLUT",
        "ECKS",
        "WHY",
        "JARED",
        "BRIAN",
        "EARLICK",
    };

    ///sadly these need to be vaguely sensible
    std::vector<std::string> classnames =
    {
        "FIGHTER", ///fine. High damage direct
        "PRIEST", ///fine. Heal teammates
        "WIZARD", ///fine. AOE damage
        "BARD", ///Done. Blocks damage to teammates at very low chance
        "RANGER", ///fineish. High dodge, hard to hit
        "DOG", ///Basically a crap ranger
        //"STAFF" ///better at being general staff?
    };

    std::map<std::string, std::string> class_to_damage_stat =
    {
        {"FIGHTER", "STR"},
        {"PRIEST", "WIS"},
        {"WIZARD", "INT"},
        {"BARD", "CHA"},
        {"RANGER", "DEX"},
        {"DOG", "DEX"},
    };

    ///with the way health and damage go atm, fighter is objective the best class
    ///we can remedy this by allowing the other classes to do other things
    std::map<std::string, float> damage_stat_to_damage_mult =
    {
        {"STR", 1.f},
        {"WIS", 0.8f},
        {"INT", 0.5f},
        {"CHA", 0.7f},
        {"DEX", 0.8f},
        {"CON", 0.6f}, ///?
    };

    std::map<std::string, float> primary_stat_to_hp_mult =
    {
        {"STR", 1.f},
        {"WIS", 0.7f},
        {"INT", 0.6f},
        {"CHA", 0.8f},
        {"DEX", 0.9f},
        {"CON", 1.2f}, ///?
    };

    std::map<std::string, float> class_damage_mult =
    {
        {"FIGHTER", 1.f},
        {"PRIEST", 1.f},
        {"WIZARD", 1.f},
        {"BARD", 1.f},
        {"RANGER", 1.f},
        {"DOG", 0.4f},
    };

    std::map<std::string, float> class_hp_mult =
    {
        {"FIGHTER", 1.f},
        {"PRIEST", 1.f},
        {"WIZARD", 1.f},
        {"BARD", 1.f},
        {"RANGER", 1.f},
        {"DOG", 0.4f},
    };

    ///1 con = con_to_hp hp
    float con_to_hp = 0.1f;

    ///needs to be kept brief, monsters must have consistently defined stats
    std::vector<std::string> monsternames =
    {
        "BEAR",
        "GOBLIN",
        "TROLL",
        "SLIME"
    };

    std::map<std::string, std::string> monstername_to_class =
    {
        {"BEAR", "FIGHTER"},
        {"GOBLIN", "RANGER"},
        {"TROLL", "FIGHTER"},
        {"SLIME", "FIGHTER"},
    };

    std::vector<std::string> weapon_class =
    {
        "SWORD",
        "BOW",
        "WIZARD STAFF",
        "MACE",
        "KNUCKLE DUSTERS",
        "LUTE" ///smack that bitch p
    };

    float weapon_damage_max = 0.2f;

    ///rand^weapon_find_power
    float weapon_find_power = 3.f;

    ///name potions after alcoholic drinks
    std::vector<std::string> item_class =
    {
        "POTION",
        "AMULET",
        "RING",
        "HAT",
        "GLOVES",
        "CHESTPLATE",
        "LEGGINGS"
    };

    std::map<int, std::string> weapon_class_to_primary_stat =
    {
        {0, "STR"},
        {1, "DEX"},
        {2, "INT"},
        {3, "STR"},
        {4, "WIS"},
        {5, "CHA"},
    };

    std::vector<std::string> item_rarity =
    {
        "Common",
        "Good Quality",
        "Uncommon",
        "Rare",
        "Ultra rare",
        "Holy crap!"
    };

    ///basically wear level - maintenance
    std::vector<std::string> weapon_wear =
    {
        "PRISTINE",
        "GOOD",
        "USED",
        "WORN",
        "BATTERED",
        "RUSTED"
    };

    std::vector<std::string> weapon_notoriety
    {
        "GOOD",
        "JUST A PINCH",
        "BAD",
        "A PINCH MORE",
        "UGLY",
    };

    float damage_to_hp_conversion = 0.2f;

    ///maximum dodge stat = 40% dodge
    float dodge_stat_to_percent_dodge = 0.02f;
    float defence_stat_to_percent_block = 0.06f;
    float cha_intercept_to_percent_block = 0.01f;

    float damage_taken_through_block = 0.3f;

    ///4% per dex
    float dex_to_dodge_chance = 0.04f;

    float hpdamage_to_healing_conversion = 0.015f;

    ///ie we get 10 turns of healing, whatever else might happen
    int turns_between_fights = 10;

    float first_level_xp = 40;
    float xp_curve = 1.1f;
}

struct stattable
{
    std::vector<base_stat> stats;

    bool is_primary(const std::string& key)
    {
        for(auto& i : stats::stat_names)
        {
            if(i == key)
            {
                return true;
            }
        }

        return false;
    }

    bool is_secondary(const std::string& key)
    {
        for(auto& i : stats::secondary_stats)
        {
            if(i == key)
            {
                return true;
            }
        }

        return false;
    }

    void add(const std::vector<base_stat>& st)
    {
        for(int i=0; i<st.size(); i++)
        {
            for(int j=0; j<stats.size(); j++)
            {
                if(st[i].key == stats[j].key)
                {
                    stats[j].val += st[i].val;
                }
            }
        }
    }

    int stat_id(const std::string& key)
    {
        for(int i=0; i<stats.size(); i++)
        {
            if(stats[i].key == key)
            {
                return i;
            }
        }

        printf("Error in stat_id no key %s\n", key.c_str());

        return -1;
    }

    float get_stat_val(const std::string& key)
    {
        for(auto& i : stats)
        {
            if(i.key == key)
            {
                return i.val;
            }
        }

        return -1.f;
    }

    float modify_stat_val(const std::string& key, float diff)
    {
        for(auto& i : stats)
        {
            if(i.key == key)
            {
                i.val += diff;

                return i.val;
            }
        }

        return -1.f;
    }

    void set_stat_val(const std::string& key, float value)
    {
        for(auto& i : stats)
        {
            if(i.key == key)
            {
                i.val = value;
            }
        }
    }

    std::string stat_display()
    {
        std::string ret;

        for(auto& i : stats)
        {
            if(i.val > 0)
            {
                ret = ret + "+" + to_string_prec((int)i.val, 3) + " " + i.key + " ";
            }
        }

        return ret;
    }

    float get_total()
    {
        float v = 0.f;

        for(auto& i : stats)
        {
            v += i.val;
        }

        return v;
    }

    void init_stats(float to_what)
    {
        stats = stats::default_stats(to_what);
    }

    float distance_from(const std::string& key, float val)
    {
        float fv = get_stat_val(key);

        return fv - val;
    }
};

#endif // STATS_HPP_INCLUDED
