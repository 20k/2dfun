#ifndef SCENARIO_CONSTRUCTOR_HPP_INCLUDED
#define SCENARIO_CONSTRUCTOR_HPP_INCLUDED

#include "shared.hpp"

namespace scenarios
{
    static std::vector<int> max_xp_per_fight_per_difficulty_level
    {
        40,
        80,
        120,
        240,
        480
    };

    //int num_types = 3;

    static std::vector<std::string> types
    {
        "MOUNTAINS",
        "JUNGLE",
        "MINES",
        "CAVES"
    };

    ///Ok. We don't really want just one kind of monster per area
    ///perhaps it should just be a weighting?
    ///Ok, Probability weighting for each monster in the monster bits
    ///Not absolute probability, monster ratio
    static std::map<std::string, std::vector<float>> type_to_monsters
    {
        {"MOUNTAINS", {1.f, 0.3f, 0.5f, 0.1f}},
        {"JUNGLE", {0.3f, 1.f, 0.3f, 0.5f}},
        {"MINES", {0.1f, 0.4f, 1.f, 0.3f}},
        {"CAVES", {0.8f, 0.6f, 0.7f, 1.f}}, ///caves is most variable
    };

    ///very VERY placeholder
    static std::vector<int> difficulty_to_reward_num
    {
        8,
        16,
        32,
        32,
        64
    };

    ///this is simpler and more controllable than a procedurally generated scheme
    static std::vector<std::vector<float>> difficulty_to_item_rarity_ratio
    {
        {1.f, 0.05f, 0.001f, 0.f, 0.f, 0.f},
        {0.8, 0.2f, 0.01f, 0.001f, 0.f, 0.f},
        {0.6f, 0.4f, 0.1f, 0.01f, 0.001f, 0.f},
        {0.4f, 0.6f, 0.2f, 0.1f, 0.01f, 0.001f},
        {0.2f, 0.4f, 0.3f, 0.2f, 0.05f, 0.01f},
    };
}

///heal in between stops, dependent on priest and potions?
///currently makes more sense for the scenario manager to own characters
///distribute loot
struct scenario_manager
{
    std::vector<entity_manager> fights;
    std::vector<character*> chars; ///general entities

    std::vector<character*> players_backup;

    ///0 -> types.size()-1
    int scenario_type = 0;

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

    int difficulty = 0;
    int stages = 0;
    int type = 0;

    int current_stage = 0;

    ///difficulty = 0 -> 4
    void init(int _difficulty, int _stages, int _type)
    {
        difficulty = _difficulty;
        stages = _stages;

        type = _type;
    }

    void random_type()
    {
        type = randf<1, int>(0, scenarios::type_to_monsters.size());
    }

    std::string get_scenario_name()
    {
        return scenarios::types[type];
    }

    int get_scenario_stages()
    {
        return 2;
    }

    float get_monster_probability_divisor()
    {
        float v = 0;

        for(auto& i : scenarios::type_to_monsters[get_scenario_name()])
        {
            v += i;
        }

        return v;
    }

    float get_difficulty_probability_divisor()
    {
        float v = 0;

        for(auto& i : scenarios::difficulty_to_item_rarity_ratio[difficulty])
        {
            v += i;
        }

        return v;
    }

    int get_monster_type()
    {
        return randf<1, int>(0, stats::monsternames.size());
    }

    int get_monster_num()
    {
        return 8;
    }

    int get_random_monster()
    {
        float divs = get_monster_probability_divisor();

        float rand_val = randf_s(0.f, 1.f);

        float cp = 0.f;

        for(int i=0; i<scenarios::type_to_monsters[get_scenario_name()].size(); i++)
        {
            cp += scenarios::type_to_monsters[get_scenario_name()][i];

            if(rand_val <= cp / divs)
                return i;
        }

        printf("This should be impossible\n");

        return -1;
    }

    int get_random_rarity()
    {
        float divs = get_difficulty_probability_divisor();

        float rand_val = randf_s(0.f, 1.f);

        float cp = 0.f;

        for(int i=0; i<scenarios::difficulty_to_item_rarity_ratio[difficulty].size(); i++)
        {
            cp += scenarios::difficulty_to_item_rarity_ratio[difficulty][i];

            if(rand_val <= cp / divs)
                return i;
        }

        printf("This should be impossible in random rarity\n");

        return 0;
    }

    std::vector<int> get_monster_types(int num)
    {
        std::vector<int> ret;

        for(int i=0; i<num; i++)
        {
            ret.push_back(get_random_monster());
        }

        return ret;
    }

    std::string get_monster_name(int type)
    {
        return stats::monsternames[type];
    }

    std::string get_monster_class(const std::string& name)
    {
        return stats::monstername_to_class[name];
    }

    void insert_party(const std::vector<character*>& cs)
    {
        players_backup = cs;
    }

    void insert_party(entity_manager& manage)
    {
        players_backup = manage.chars;
    }

    void begin_fight()
    {
        fights.push_back(entity_manager());

        entity_manager& manage = fights.back();

        int monster_num = get_monster_num();

        std::vector<int> monster_types = get_monster_types(monster_num);

        for(int i=0; i<monster_num; i++)
        {
            int monster_type = monster_types[i];

            character* monst = manage.make_new(1);

            monst->rand_manual_classname(get_monster_name(monster_type), get_monster_class(get_monster_name(monster_type)), "STR", difficulty);

            if(difficulty == 0)
                monst->delevel_fully();
        }

        for(auto& i : players_backup)
        {
            manage.insert_character(i);
        }
    }

    void complete_fight()
    {
        int c = 0;

        while(!fights.back().fight_over() && c < 40)
        {
            fights.back().resolve_half_turn();

            c++;
        }

        dish_out_xp();

        rest();
    }

    void fully_resolve_scenario()
    {
        int stages = get_scenario_stages();

        for(int i=0; i<stages; i++)
        {
            begin_fight();

            complete_fight();
        }
    }

    ///automatically called
    void rest()
    {
        for(int i=0; i<stats::turns_between_fights; i++)
        {
            fights.back().idle_turn();
        }
    }

    bool is_player(character* c)
    {
        for(auto& i : players_backup)
        {
            if(i == c)
                return true;
        }

        return false;
    }

    ///lumpy xp, or even split?
    void dish_out_xp()
    {
        float xp = 0.f;

        for(character* i : fights.back().chars)
        {
            if(!is_player(i))
            {
                xp += i->get_difficulty();
            }
        }

        ///only distribute xp for alive players?
        for(character* i : players_backup)
        {
            i->add_xp(xp / players_backup.size());
        }
    }

    std::vector<item*> distribute_loot(item_manager& item_manage)
    {
        int num_items = scenarios::difficulty_to_reward_num[difficulty];

        std::vector<item*> ret;

        for(int i=0; i<num_items; i++)
        {
            item* it = item_manage.make_new();

            it->random_item();

            int rare = get_random_rarity();

            it->random_magical(rare);

            ret.push_back(it);
        }

        return ret;
    }
};

#endif // SCENARIO_CONSTRUCTOR_HPP_INCLUDED
