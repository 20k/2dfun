#include <iostream>
#include <vector>
#include <stdint.h>
#include <string>
#include <map>
#include <net/shared.hpp>
#include <vec/vec.hpp>
#include <sstream>
#include <iomanip>
#include "shared.hpp"

#include "stats.hpp"
#include "item.hpp"
#include "inventory.hpp"
#include "entities.hpp"
#include "economics.hpp"
#include "scenario_constructor.hpp"

/*std::vector<base_stat> combine(const std::vector<base_stat>& b1, const std::vector<base_stat>& b2)
{

}*/


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
    monster_char->delevel_fully();

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
