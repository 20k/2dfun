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
#include "shop.hpp"
#include "draw_manager.hpp"
#include "drag_manager.hpp"
#include "shop_general_manager.hpp"
#include "world.hpp"

/*std::vector<base_stat> combine(const std::vector<base_stat>& b1, const std::vector<base_stat>& b2)
{

}*/

///resting, levelling (new xp stuff!), multi phase attacks
int main()
{
    drag_manager drag_manage;

    item_manager item_manage;


    item* nitem2 = item_manage.make_new();

    item* nitem = item_manage.make_new();

    //nitem.init_weapon_class(0, 0.02f);
    //nitem->random_stat_appropriate_weapon("CHA");
    nitem->random_magical(2);
    //nitem->accumulated_wear+=5;
    nitem2->random_magical(1);
    nitem2->random_item();

    entity_manager buyable_entities;

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
    //nitem2->random_stat_appropriate_weapon(p4->primary_stat);
    p4->add_to_invent(nitem);

    /*for(int i=0; i<6; i++)
    {
        std::cout << nitem2->item_class << " " << p4->add_to_invent(nitem2) << std::endl;
    }*/


    //bool added = p4->add_to_invent(nitem2);

    //std::cout << "poo " << added << std::endl;
    //p4->add_xp(58);

    std::cout << "pdump" << std::endl;

    std::cout << p1->display() << std::endl;
    std::cout << p2->display() << std::endl;
    std::cout << p3->display() << std::endl;
    std::cout << p4->display() << std::endl;

    std::cout << "epd" << std::endl;

    /*scenario_manager doom;
    doom.init(0, 2, 0);

    doom.insert_party(party);

    //doom.begin_fight();
    //doom.complete_fight();

    doom.fully_resolve_scenario();*/

    //p3->add_xp(21.f + 84 + 324);

    std::cout << std::endl << party.display_critical() << std::endl;

    draw_manager draw_manage;
    draw_manage.init(1600, 900);

    ImGui::SFML::Init(draw_manage.window);

    shop_general_manager shop_general;

    shop_general.init(&item_manage, {30*9, 30*9}, 30);

    /*for(int i=0; i<stats::starting_items; i++)
    {
        item* ni = item_manage.make_new();

        ni->random_item_or_weapon();

        shop_general.shop_manage.make_sellable(ni);

        //std::cout << ni->display() << std::endl;
    }*/

    /*auto items = doom.distribute_loot(item_manage);

    for(auto& i : items)
    {
        shop_general.shop_manage.make_sellable(i);
    }*/

    world world_state;

    sf::Clock party_time;

    while(draw_manage.window.isOpen())
    {
        world_state.register_as_active_party(&party);

        std::vector<item*> unclaimed = world_state.claim_unclaimed_items();

        for(auto& i : unclaimed)
        {
            shop_general.shop_manage.make_sellable(i);
        }

        draw_manage.tick();

        world_state.tick(draw_manage.get_frametime_s());

        shop_general.tick(draw_manage.get_frametime_s(), draw_manage);
        shop_general.draw_tiles(draw_manage);

        draw_manage.draw_entity_ui(party, drag_manage);

        shop_general.draw_shop_ui(draw_manage, drag_manage);

        ///can't figure out a way not to pass the item manager here
        world_state.draw_mission_ui(draw_manage, &item_manage);

        drag_manage.tick_entity_grab(party, shop_general.shop_manage);
        drag_manage.tick();

        draw_manage.render_ui();
        draw_manage.flip();

        if(party_time.getElapsedTime().asMicroseconds() / 1000.f / 1000.f >= stats::half_time_time_s)
        {
            party_time.restart();

            party.idle_turn(false);
        }
    }

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
