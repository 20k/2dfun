#include "peon_manager.hpp"
#include "stats.hpp"
#include "shop.hpp"
#include "draw_manager.hpp"

void peon::init(int ptier)
{
    tier = clamp(ptier, 0, stats::peon_class_money_base.size());

    float money = stats::peon_class_money_base[tier];

    money = money + money * randf_s(-stats::peon_money_variation, stats::peon_money_variation);

    wallet = money;
}

void peon::seek_random_item(shop& s)
{
    /*for(sellable* i : s.for_sale)
    {

    }*/

    if(s.for_sale.size() == 0)
        return;

    std::vector<tile> tiles = s.get_table_tiles();

    if(tiles.size() == 0)
        return;

    std::vector<sellable*> purchaseable_sellables = s.get_purchasable_sellables_on_tables();

    if(purchaseable_sellables.size() == 0)
        return;

    bool success = false;

    int c = 0;
    int max_tries = 5;

    while(!success && c < max_tries)
    {
        c++;

        int rseek = randf<1, int>(0, (int)purchaseable_sellables.size());

        sellable* seek = purchaseable_sellables[rseek];

        if(seek->listed_price >= wallet || seek->locked)
            continue;

        currently_seeking = seek;
        currently_seeking->locked = true;
    }
}

bool peon::within_purchase_distance_of_currently_seeking(shop& s)
{
    sellable* c = currently_seeking;

    vec2i tile_pos = s.sellable_to_tile(c);

    vec2f world_pos = pos;

    vec2f grid_pos = world_pos / s.grid_dim;

    vec2f dist = conv_implicit<vec2f>(tile_pos) - grid_pos;

    float len = dist.length();

    printf("%f buydist\n", len);

    if(len < 0.5)
        return true;

    return false;
}

bool peon::can_purchase_currently_seeking(shop& s)
{
    bool is_within_dist = within_purchase_distance_of_currently_seeking(s);

    if(is_within_dist && currently_seeking->listed_price < wallet)
    {
        return true;
    }

    return false;
}

bool peon::try_purchase_currently_seeking(shop& s)
{
    if(!can_purchase_currently_seeking(s))
        return false;

    wallet -= currently_seeking->listed_price;

    ///add to peon invent?
    s.purchase(currently_seeking);

    currently_seeking = nullptr;

    return true;
}

void peon::pathfind(shop& s, float dt_s)
{
    vec2i target_grid = s.sellable_to_tile(currently_seeking);

    vec2f dest = (vec2f){target_grid.x(), target_grid.y()} * (float)s.grid_dim;

    vec2f cur = pos;

    float md = stats::peon_move_speed * dt_s;

    float dist = (dest - cur).length();

    if(dist < md)
        md = dist;

    if(dist < 0.1f)
        return;

    vec2f dir = (dest - cur).norm();

    cur = cur + dir * md;

    pos = cur;
}

peon* peon_manager::make_peon()
{
    peon* p = new peon;

    peons.push_back(p);

    return p;
}

void peon_manager::remove_peon(peon* p)
{
    for(int i=0; i<peons.size(); i++)
    {
        if(peons[i] == p)
        {
            peons.erase(peons.begin() + i);
            i--;
            delete p;
        }
    }
}

///need to move towards seek
void peon_manager::tick(shop& s, draw_manager& draw_manage)
{
    for(peon* i : peons)
    {
        if(i->currently_seeking)
        {
            bool success = i->try_purchase_currently_seeking(s);

            if(!success)
                i->pathfind(s, draw_manage.get_frametime_s());

            continue;
        }

        i->seek_random_item(s);
    }
}

void peon_manager::draw_peons(draw_manager& draw_manage)
{
    sf::Color col(0, 0, 255);
    sf::CircleShape shape;
    shape.setFillColor(col);

    float rad = 3.f;

    shape.setRadius(rad);
    shape.setOrigin(rad, rad);

    for(peon* p : peons)
    {
        vec2f pos = p->pos;

        shape.setPosition({pos.x(), pos.y()});

        draw_manage.window.draw(shape);
    }
}
