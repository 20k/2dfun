#ifndef PEON_MANAGER_HPP_INCLUDED
#define PEON_MANAGER_HPP_INCLUDED

#include <vec/vec.hpp>

struct peon_manager;
struct shop;
struct sellable;
struct draw_manager;
struct tile;

/*namespace peon_command
{
    enum commands
    {
        SEEK,
        WAIT,
        IDLE_SEEK,
        DISPLAY_SEEK, ///seek something fun
    }
}*/

///make a command queue for peons that we can insert into, that would be better than the weird status we have atm

///we need idle pathfinding next
struct peon
{
    float idling_time_s = FLT_MAX;
    float time_since_spawn_s = 0.f;

    vec2f pathfinding_destination;
    bool should_pathfind = false;

    bool currently_leaving = false;

    vec2f pos = {0, 0};
    float wallet = 0.f;
    int tier = 0;

    sellable* currently_seeking = nullptr;

    ///specific weapon interest
    ///happiness
    ///path position random
    ///calculate environmental modifiers

    ///0 -> 4?
    void init(int ptier);

    void seek_random_item(shop& s);

    bool within_purchase_distance_of_currently_seeking(shop& s);

    bool can_purchase_currently_seeking(shop& s);

    bool try_purchase_currently_seeking(shop& s);

    bool is_currently_seeking(sellable* s);

    vec2i get_random_table_display(shop& s);

    ///to target
    void pathfind(shop& s, float dt_s);
    void set_pathfind(vec2f p);
    void cancel_pathfind();

    bool should_leave(shop& s);
    bool should_idle(shop& s);

    void idle_pathfind(shop& s);
    void table_display_pathfind(shop& s);

    bool within_door(shop& s);

    void force_unseek(sellable* s);
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
