#ifndef PEON_MANAGER_HPP_INCLUDED
#define PEON_MANAGER_HPP_INCLUDED

#include <vec/vec.hpp>

struct peon_manager;
struct shop;
struct sellable;
struct draw_manager;
struct tile;

namespace peon_command
{
    enum commands
    {
        NONE,
        SEEK,
        WAIT,
        RESET_IDLE_TIME,
        IDLE_SEEK,
        DISPLAY_SEEK, ///seek something fun
        POTENTIALLY_PURCHASE, ///random chance to purchase
        DEFINITELY_PURCHASE, ///yup we're gunna buy a thing
        LEAVE
    };
}

struct command_element
{
    peon_command::commands command;

    sellable* currently_seeking = nullptr;
    vec2f pathfinding_destination;
    float cancel_dist = 0.1f;
    float dur_s = 0.f;
};

///make a command queue for peons that we can insert into, that would be better than the weird status we have atm

///we need idle pathfinding next
struct peon
{
    float buy_threshold = 0.f;

    float idling_time_s = FLT_MAX;
    float time_since_spawn_s = 0.f;

    /*vec2f pathfinding_destination;
    bool should_pathfind = false;

    bool currently_leaving = false;

    sellable* currently_seeking = nullptr;*/

    std::vector<command_element> command_list;

    vec2f pos = {0, 0};
    float wallet = 0.f;
    int tier = 0;

    ///specific weapon interest
    ///happiness
    ///path position random
    ///calculate environmental modifiers

    ///0 -> 4?
    void init(int ptier);

    /*void seek_random_item(shop& s);*/
    bool might_buy(sellable* s);

    bool within_purchase_distance_of_currently_seeking(shop& s);

    bool can_purchase_currently_seeking(shop& s);

    bool try_purchase_currently_seeking(shop& s);

    /*bool is_currently_seeking(sellable* s);


    ///to target
    void pathfind(shop& s, float dt_s);
    void set_pathfind(vec2f p);
    void cancel_pathfind();

    bool should_leave(shop& s);
    bool should_idle(shop& s);

    void idle_pathfind(shop& s);
    void table_display_pathfind(shop& s);*/

    vec2f get_idle_position(shop& s);

    vec2i get_random_table_display(shop& s);

    sellable* get_random_item_at_table(vec2i tab, shop& s);

    bool pathfind(shop& s, float dt_s, float cancel_dist);

    void tick(shop& s, draw_manager& draw_manage);

    bool within_door(shop& s);

    void force_unseek(sellable* s);

    command_element get_current_command();
    bool is_command_type(peon_command::commands c);

    bool no_commands();

    void pop_front_command();

    bool is_leaving();
};

struct peon_manager
{
    std::vector<peon*> peons;

    peon* make_peon();

    void remove_peon(peon* p);

    void tick(shop& s, draw_manager& draw_manage);

    void draw_peons(draw_manager& draw_manage);

    void force_unseek(sellable* s);
};

#endif // PEON_MANAGER_HPP_INCLUDED
