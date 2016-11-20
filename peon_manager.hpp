#ifndef PEON_MANAGER_HPP_INCLUDED
#define PEON_MANAGER_HPP_INCLUDED

#include <vec/vec.hpp>

struct peon_manager;
struct shop;
struct sellable;
struct draw_manager;

struct peon
{
    vec2f pathfinding_destination;
    bool should_pathfind = false;

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

    ///to target
    void pathfind(shop& s, float dt_s);
    void set_pathfind(vec2f p);

    bool should_leave(shop& s);

    bool within_door(shop& s);
};

struct peon_manager
{
    std::vector<peon*> peons;

    peon* make_peon();

    void remove_peon(peon* p);

    void tick(shop& s, draw_manager& draw_manage);

    void draw_peons(draw_manager& draw_manage);
};

#endif // PEON_MANAGER_HPP_INCLUDED
