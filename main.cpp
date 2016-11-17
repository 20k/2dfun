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

///resting, levelling (new xp stuff!), multi phase attacks
int main()
{
    item_manager item_manage;

    item* nitem = item_manage.make_new();

    //nitem.init_weapon_class(0, 0.02f);
    //nitem->random_stat_appropriate_weapon("CHA");
    nitem->random_magical(2);

    entity_manager party;

    character* p1 = party.make_new(0);

    p1->rand_stats();

    character* p2 = party.make_new(0);

    p2->rand_stats();

    character* p3 = party.make_new(0);

    p3->rand_stats();

    character* p4 = party.make_new(0);

    p4->rand_stats();

    nitem->random_stat_appropriate_weapon(p4->primary_stat);
    //p4->add_to_invent(nitem);

    std::cout << "pdump" << std::endl;

    std::cout << p1->display() << std::endl;
    std::cout << p2->display() << std::endl;
    std::cout << p3->display() << std::endl;
    std::cout << p4->display() << std::endl;

    std::cout << "epd" << std::endl;

    scenario_manager doom;

    doom.insert_party(party);

    doom.begin_fight();
    doom.complete_fight();

    //p3->add_xp(21.f + 84 + 324);

    std::cout << std::endl << party.display_critical() << std::endl;


    #if 0
    for(int i=0; i<12; i++)
        rand();

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
    #endif

    return 0;
}
