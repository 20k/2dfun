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

    ///with the way health and damage go atm, fighter is objective the best class
    ///we can remedy this by allowing the other classes to do other things
    std::map<std::string, float> damage_stat_to_damage_mult =
    {
        {"STR", 1.f},
        {"WIS", 0.8f},
        {"INT", 0.9f},
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

    std::vector<std::string> weapon_class =
    {
        "SWORD",
        "BOW",
        "WIZARD STAFF",
        "MACE",
        "KNUCKLE DUSTERS",
        "LUTE" ///smack that bitch p
    };

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

    float damage_to_hp_conversion = 0.3f;
}

struct stattable
{
    std::vector<base_stat> stats;

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

    std::string stat_display()
    {
        std::string ret;

        for(auto& i : stats)
        {
            if(i.val > 0)
            {
                ret = ret + "+" + std::to_string((int)i.val) + " " + i.key + " ";
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
        for(int i=0; i<(int)stats::stat_names.size(); i++)
        {
            stats.push_back({stats::stat_names[i], to_what});
        }
    }
};

struct item : stattable
{
    ///times base_stat
    float attack_boost_hp_flat = 0.f;

    ///(character stat + stat_boost)/10 * attack_boost_hp_flat
    std::string primary_stat;
    std::string item_class;
    std::string description;
    int weapon_class = -1;

    void set_attack_boost(float hp)
    {
        attack_boost_hp_flat = hp;
    }

    item()
    {
        init_stats(0.f);
    }

    void init_weapon_class(int id, float extra_hp_damage)
    {
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

        ret = item_class;

        if(stat_string.size() > 0)
            ret = ret + " of " + stat_display() + "\n";// + description;

        std::string first_letter = stat_string.size() > 0 ? "D" : " d";

        if(weapon_class != -1)
        {
            ret = ret + first_letter + "ealing an extra " + std::to_string(attack_boost_hp_flat) + " hp of damage\n";
        }

        ret = ret + description;

        return ret;
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
    }
};

struct item_manager
{
    std::vector<item*> items;

    item* make_new()
    {

    }
};

struct combat_entity
{
    float hp = 1.f;
    float hp_max = 1.f;
    int team = 0;

    //virtual void init();

    virtual float calculate_damage();

    ///me->calculate_damage() / them->calculate_def_damage_divisor
    virtual float calculate_def_damage_divisor();

    virtual void attack(combat_entity* entity)
    {
        float my_damage = calculate_damage();

        float their_def = entity->calculate_def_damage_divisor();

        float dam = my_damage / their_def;

        dam *= stats::damage_to_hp_conversion;

        entity->modify_hp(-dam);
    }

    virtual ~combat_entity()
    {

    }

    void modify_hp(float hp_change)
    {
        hp += hp_change;
    }

    void set_team(int _team)
    {
        team = _team;
    }
};

struct character : combat_entity, stattable
{
    int cur_level = 1;

    std::string name;
    std::string classname;
    std::string primary_stat;
    std::string race;

    character()
    {
        init_stats(10.f);
    }

    void recalculate_hp()
    {
        ///old /hp_max = stats::primary_stat_to_hp_mult[primary_stat];

        hp_max = stats::con_to_hp * get_stat_val("CON") * stats::primary_stat_to_hp_mult[primary_stat];
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

        recalculate_hp();
        hp = hp_max;
    }

    void rand_manual_classname(const std::string& custom_classname, const std::string& pprimary_stat, int rebal_stat = 0)
    {
        rand_stats();

        classname = custom_classname;

        primary_stat = pprimary_stat;

        rebalance(primary_stat, rebal_stat);

        recalculate_hp();
        hp = hp_max;
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

        str = str + "Name: " + name + "\n";
        str = str + "Class: " + classname + "\n";

        for(auto& i : stats)
        {
            str = str + i.key + " " + std::to_string(i.val) + "\n";

            total += i.val;
        }

        str = str + "Total: " + std::to_string(total) + "\n";

        str = str + "HP: " + std::to_string(hp) + "/" + std::to_string(hp_max) + "\n";

        if(is_dead())
        {
            str = str + "DEAD RUH ROH\n";
        }

        return str;
    }

    void level(int level_stat)
    {
        stats[level_stat].val += 1.f;

        recalculate_hp();
    }

    void auto_level()
    {
        std::string st;

        st = (randf_s(0.f, 1.f) < 0.5f) ? "CON" : primary_stat;

        level(stat_id(st));
    }

    bool is_dead()
    {
        return hp <= 0.f;
    }

    float get_difficulty()
    {
        //std::cout << "psfval " + std::to_string(get_stat_val(primary_stat));

        return get_total() * 0.1f + get_stat_val(primary_stat) + get_stat_val("CON") * 2 * stats::primary_stat_to_hp_mult[primary_stat];
    }
};

struct entity_manager
{
    std::vector<character*> chars;

    int half_turn_counter = 0;

    character* make_new(int team)
    {
        character* c = new character;

        //c->init();
        c->set_team(team);

        //c->rand_stats();

        chars.push_back(c);

        return c;
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

    void resolve_half_turn()
    {
        if(fight_over())
        {
            printf("Fight over\n");
            return;
        }

        std::map<int, std::vector<character*>> team_to_chars;

        for(auto& i : chars)
        {
            team_to_chars[i->team].push_back(i);
        }

        int cteam = half_turn_counter % 2;
        int other_team = 1 - cteam;

        int other_team_num = team_to_chars[other_team].size();

        for(int i=0; i<team_to_chars[cteam].size(); i++)
        {
            character* ccharacter = team_to_chars[cteam][i];

            if(ccharacter->is_dead())
                continue;

            int random_enemy = -1;
            character* enemy = nullptr;
            bool any = false;

            for(int kk=0; kk < other_team_num; kk++)
            {
                random_enemy = randf<1, int>(0, other_team_num);
                enemy = team_to_chars[other_team][random_enemy];

                if(!enemy->is_dead())
                {
                    any = true;
                    break;
                }
            }

            if(!any)
                continue;

            ccharacter->attack(enemy);
        }

        for(auto& i : chars)
        {
            std::cout << i->display() << std::endl;
        }

        half_turn_counter++;
    }

    bool fight_over()
    {
        return all_dead(get_team(0)) || all_dead(get_team(1));
    }
};

int main()
{
    entity_manager entity_manage;

    character* base_char = entity_manage.make_new(0);

    base_char->rand_stats();

    character* base_char2 = entity_manage.make_new(0);

    base_char2->rand_stats();

    character* monster_char = entity_manage.make_new(1);

    monster_char->rand_manual_classname("BOAR", "STR", 2);

    monster_char->auto_level();

    entity_manage.resolve_half_turn();
    entity_manage.resolve_half_turn();
    entity_manage.resolve_half_turn();
    entity_manage.resolve_half_turn();
    entity_manage.resolve_half_turn();
    entity_manage.resolve_half_turn();
    entity_manage.resolve_half_turn();
    entity_manage.resolve_half_turn();
    entity_manage.resolve_half_turn();
    entity_manage.resolve_half_turn();
    entity_manage.resolve_half_turn();
    entity_manage.resolve_half_turn();
    entity_manage.resolve_half_turn();
    entity_manage.resolve_half_turn();
    entity_manage.resolve_half_turn();
    entity_manage.resolve_half_turn();
    entity_manage.resolve_half_turn();

    item nitem;

    nitem.init_weapon_class(0, 0.02f);
    nitem.random_magical(2);

    std::cout << nitem.display() << std::endl;

    std::cout << std::to_string(monster_char->get_difficulty()) + "xp" << std::endl;

    /*std::cout << monster_char->display() << std::endl;

    monster_char->auto_level();

    std::cout << monster_char->display() << std::endl;

    monster_char->auto_level();

    std::cout << monster_char->display() << std::endl;

    monster_char->auto_level();

    std::cout << monster_char->display() << std::endl;

    monster_char->auto_level();

    std::cout << monster_char->display() << std::endl;*/

    return 0;
}
