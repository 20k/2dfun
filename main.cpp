#include <iostream>
#include <vector>
#include <stdint.h>
#include <string>
#include <map>
#include <net/shared.hpp>
#include <vec/vec.hpp>

struct base_stat
{
    std::string key;
    float val = 10.f;
};

namespace stats
{
    std::vector<std::string> stat_names =
    {
        "STR",
        "CON",
        "DEX",
        "INT",
        "WIS",
        "CHA"
    };

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
        "QUARTERLING"
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
        "BREAD"
    };

    ///sadly these need to be vaguely sensible
    std::vector<std::string> classnames =
    {
        "FIGHTER",
        "PRIEST",
        "WIZARD",
        "BARD",
        "RANGER",
        //"STAFF" ///better at being general staff?
    };

    std::map<std::string, std::string> class_to_damage_stat =
    {
        {"FIGHTER", "STR"},
        {"PRIEST", "WIS"},
        {"WIZARD", "INT"},
        {"BARD", "CHA"},
        {"RANGER", "DEX"},
    };

    std::map<std::string, float> damage_stat_to_damage_mult =
    {
        {"STR", 1.f},
        {"WIS", 0.8f},
        {"INT", 0.9f},
        {"CHA", 0.7f},
        {"DEX", 0.8f},
        {"CON", 0.6f}, ///?
    };

    ///needs to be kept brief, monsters must have consistently defined stats
    std::vector<std::string> monsternames =
    {
        "BEAR",
        "GOBLIN",
        "TROLL",
        "SLIME"
    };

    float damage_to_hp_conversion = 0.1f;
}

struct combat_entity
{
    float hp = 1.f;

    virtual void init();

    virtual float calculate_damage();

    ///me->calculate_damage() / them->calculate_def_damage_divisor
    virtual float calculate_def_damage_divisor();

    virtual void attack(combat_entity* entity)
    {
        float my_damage = calculate_damage();

        float their_def = entity->calculate_def_damage_divisor();

        float dam = my_damage / their_def;

        dam *= stats::damage_to_hp_conversion;

        entity->damage(dam);
    }

    virtual ~combat_entity()
    {

    }

    void damage(float direct_hp_damage)
    {
        hp -= direct_hp_damage;
    }
};

struct character : combat_entity
{
    std::vector<base_stat> stats;

    std::string name;
    std::string classname;
    std::string primary_stat;
    std::string race;

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

    void init()
    {
        for(int i=0; i<(int)stats::stat_names.size(); i++)
        {
            stats.push_back({stats::stat_names[i], 10.f});
        }
    }

    ///perhaps rand_stats_with_primary, for monsters?
    void rand_stats()
    {
        for(auto& st : stats)
        {
            vec4i rand_dicerolls = randf<4, int>(1, 7);

            vec4i sorted = rand_dicerolls.sorted();

            int val = sorted.v[1];
            val += sorted.v[2];
            val += sorted.v[3];

            st.val = val;

            //printf("stat %i %s\n", val, st.key.c_str());
        }

        race = stats::races[randf<1, int>(0, stats::races.size())];
        classname = stats::classnames[randf<1, int>(0, stats::classnames.size())]; ///after this point, flavourtext?
        name = stats::names[randf<1, int>(0, stats::names.size())];

        primary_stat = stats::class_to_damage_stat[classname];
    }

    void rand_manual_classname(const std::string& custom_classname, const std::string& pprimary_stat, int rebal_stat = 0)
    {
        rand_stats();

        classname = custom_classname;

        primary_stat = pprimary_stat;

        rebalance(primary_stat, rebal_stat);
    }

    void rebalance(const std::string& stat_into, int num)
    {
        for(int i=0; i<num; i++)
        {
            int rstat = randf<1, int>(0, stats.size());

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

        float stats_damage_mult = stats::damage_stat_to_damage_mult[key];

        return 1.f * (get_stat_val(key) / 10.f) * stats_damage_mult;
    }

    float calculate_def_damage_divisor() override
    {
        std::string key = primary_stat;

        float stats_damage_mult = stats::damage_stat_to_damage_mult[key];

        float con_mult = get_stat_val("CON") / 10.f;

        float def_stat = stats_damage_mult * con_mult;

        return def_stat;
    }

    std::string display()
    {
        std::string str;

        float total = 0.f;

        for(auto& i : stats)
        {
            str = str + i.key + " " + std::to_string(i.val) + "\n";

            total += i.val;
        }

        str = str + "Total: " + std::to_string(total) + "\n";

        return str;
    }
};

struct entity_manager
{
    std::vector<combat_entity*> chars;

    character* make_new()
    {
        character* c = new character;

        c->init();

        //c->rand_stats();

        return c;
    }

    void resolve_half_turn()
    {

    }
};

int main()
{
    entity_manager entity_manage;

    character* base_char = entity_manage.make_new();

    base_char->rand_stats();

    character* monster_char = entity_manage.make_new();

    monster_char->rand_manual_classname("Boar", "STR", 2);

    std::cout << monster_char->display() << std::endl;

    return 0;
}
