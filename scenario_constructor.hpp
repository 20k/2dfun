#ifndef SCENARIO_CONSTRUCTOR_HPP_INCLUDED
#define SCENARIO_CONSTRUCTOR_HPP_INCLUDED

#include "shared.hpp"

namespace scenarios
{
    std::vector<int> max_xp_per_fight_per_difficulty_level
    {
        40,
        80,
        120,
        240
    };
}

///heal in between stops, dependent on priest and potions?
///currently makes more sense for the scenario manager to own characters
struct scenario_manager
{
    std::vector<entity_manager> fights;
    std::vector<character*> chars; ///general entities

    std::vector<character*> players_backup;

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
                delete chars[i];

                chars.erase(chars.begin() + i);
                i--;
            }
        }
    }

    int difficulty = 0;
    int stages = 0;

    int current_stage = 0;

    void init(int _difficulty, int _stages)
    {
        difficulty = _difficulty;
        stages = _stages;
    }

    int get_monster_type()
    {
        return randf<1, int>(0, stats::monsternames.size());
    }

    int get_monster_num()
    {
        return 2;
    }

    void insert_party(const std::vector<character*>& cs)
    {
        players_backup = cs;
    }

    void begin_fight()
    {
        fights.push_back(entity_manager());

        entity_manager& manage = fights.back();

        for(int i=0; i<get_monster_num(); i++)
        {
            character* monst = manage.make_new(1);

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

        while(!entity_manage.fight_over() && c < 40)
        {
            entity_manage.resolve_half_turn();

            c++;
        }
    }
};

#endif // SCENARIO_CONSTRUCTOR_HPP_INCLUDED
