#include <iostream>
#include <vector>
#include <stdint.h>
#include <string>
#include <map>
#include <net/shared.hpp>
#include <vec/vec.hpp>
#include <sstream>
#include <iomanip>

struct base_stat
{
    std::string key;
    float val = 10.f;
};

/*std::vector<base_stat> combine(const std::vector<base_stat>& b1, const std::vector<base_stat>& b2)
{

}*/

template <typename T>
std::string to_string_prec(const T& a_value, const int n = 6)
{
    std::ostringstream out;
    out << std::setprecision(n) << a_value;
    return out.str();
}

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

        ret = ret + "Condition: " + stats::weapon_wear[get_wear()] + "\n";
        ret = ret + "Notoriety: " + stats::weapon_notoriety[get_notoriety()] + "\n";

        ret = ret + description;

        if(ret.back() != '\n')
            ret.push_back('\n');

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

        rarity += extra_stats;
    }

    void random_stat_appropriate_weapon(const std::string& stat)
    {
        int id = stat_id(stat);

        std::vector<int> appropriate_weapons;

        for(auto& i : stats::weapon_class_to_primary_stat)
        {
            if(i.second == stat)
            {
                appropriate_weapons.push_back(id);
            }
        }

        if(appropriate_weapons.size() == 0)
        {
            std::cout << "no weapons for stat " << stat << std::endl;
            return;
        }

        int random_weapon_num = randf<1, int>(0, appropriate_weapons.size());

        int random_weapon = appropriate_weapons[random_weapon_num];

        float damage = pow(randf_s(0.f, 1.f), stats::weapon_find_power) * stats::weapon_damage_max;

        init_weapon_class(random_weapon, damage);
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
};

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
};

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

    virtual bool is_dead(){};
};

struct character : combat_entity, stattable
{
    inventory invent;

    int cur_level = 1;

    std::string classname;
    std::string primary_stat;
    std::string race;

    character()
    {
        init_stats(10.f);
    }

    void register_kill() override
    {
        invent.register_kill();
    }

    float get_dodge_chance() override
    {
        return get_item_modified_stat_val("DGE") * stats::dodge_stat_to_percent_dodge  + (get_item_modified_stat_val("DEX") - 10.f) * stats::dex_to_dodge_chance;
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

        return 1.f * (get_item_modified_stat_val(key) / 10.f) * stats_damage_mult + invent.get_damage_bonus();
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

    bool is_dead() override
    {
        return hp <= 0.f;
    }

    float get_difficulty()
    {
        //std::cout << "psfval " + std::to_string(get_stat_val(primary_stat));

        return get_total() * 0.1f + get_item_modified_stat_val(primary_stat) + get_item_modified_stat_val("CON") * 2 * stats::primary_stat_to_hp_mult[primary_stat] * stats::class_hp_mult[classname] + cur_level * 3;
    }

    inventory& get_invent()
    {
        return invent;
    }

    void add_to_invent(item* i)
    {
        invent.add_item(i);

        recalculate_hp();
    }

    float get_teammate_heal()
    {
        float cur = 0.f;

        if(primary_stat == "WIS")
            cur = get_item_modified_stat_val("WIS") * stats::damage_to_hp_conversion * stats::hpdamage_to_healing_conversion;

        cur += get_item_modified_stat_val("HEAL") * stats::damage_to_hp_conversion * stats::hpdamage_to_healing_conversion;

        return cur;
    }

    ///starts to shine vs 2+ enemies
    float calculate_group_damage()
    {
        if(primary_stat != "INT")
            return 0.f;

        return (get_item_modified_stat_val(primary_stat) / 10.f) * stats::damage_stat_to_damage_mult[primary_stat] * stats::class_damage_mult[classname] + invent.get_damage_bonus() * 0.4f;
    }
};

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

    std::string process_heals(int pteam)
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
            if(randf_s(0.f, 1.f) < valid_saver_teammates[i]->get_intercept_chance())
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

        std::string str = process_heals(cteam);

        std::cout << str;

        half_turn_counter++;

        if(fight_over())
        {
            on_fight_end();
        }
    }

    void on_fight_end()
    {

    }

    bool fight_over()
    {
        return all_dead(get_team(0)) || all_dead(get_team(1));
    }
};

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
    std::vector<int> approx_economic_value
    {
        100,
        200,
        700,
        2000,
        10000,
        100000
    };

    ///value for damage scaling
    float damage_max_value = 10000.f;

    float percent_value_lost_per_wear_level = 0.1;
}

struct economic_item
{
    float value = 0.f;

    void load_from_item(item* i)
    {
        int rarity = i->rarity;

        rarity = std::min(rarity, (int)economics::approx_economic_value.size()-1);

        int nominal_value = economics::approx_economic_value[rarity];

        int value_added_damage = i->attack_boost_hp_flat * economics::damage_max_value;

        int wear = i->get_wear();

        int notoriety = i->notoriety;

        value = nominal_value + value_added_damage;

        value = value - value * wear * economics::percent_value_lost_per_wear_level;
    }
};

int main()
{
    for(int i=0; i<12; i++)
        rand();

    item_manager item_manage;

    item* nitem = item_manage.make_new();

    //nitem.init_weapon_class(0, 0.02f);
    nitem->random_stat_appropriate_weapon("CHA");
    nitem->random_magical(2);

    entity_manager entity_manage;

    character* base_char = entity_manage.make_new(0);

    //base_char->rand_manual_classname("DOG", "DOG", "DEX");
    base_char->rand_stats();

    base_char->add_to_invent(nitem);

    character* base_char2 = entity_manage.make_new(0);

    base_char2->rand_stats();

    character* base_char3 = entity_manage.make_new(0);

    base_char3->rand_stats();

    character* monster_char = entity_manage.make_new(1);

    monster_char->rand_manual_classname("BOAR", "FIGHTER", "STR", 0);

    //monster_char->auto_level();

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
    entity_manage.resolve_half_turn();
    entity_manage.resolve_half_turn();
    entity_manage.resolve_half_turn();
    entity_manage.resolve_half_turn();
    entity_manage.resolve_half_turn();
    entity_manage.resolve_half_turn();

    std::cout << nitem->display() << std::endl;

    std::cout << to_string_prec(monster_char->get_difficulty(), 3) + "xp" << std::endl;

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
