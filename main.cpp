#include <iostream>
#include <vector>
#include <stdint.h>
#include <string>
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
        "STAFF" ///better at being general staff?
    };

    ///needs to be kept brief, monsters must have consistently defined stats
    std::vector<std::string> monsternames =
    {
        "BEAR",
        "GOBLIN",
        "TROLL",
        "SLIME"
    }
}

struct character
{
    std::vector<base_stat> stats;

    std::string name;
    std::string classname;
    std::string race;

    void init()
    {
        for(int i=0; i<(int)stats::stat_names.size(); i++)
        {
            stats.push_back({stats::stat_names[i], 10.f});
        }
    }

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
        classname = stats::classnames[randf<1, int>(0, stats::classnames.size())];
        name = stats::names[randf<1, int>(0, stats::names.size())];
    }
};

int main()
{
    character base_char;
    base_char.init();
    base_char.rand_stats();

    return 0;
}
