#include "peon_manager.hpp"
#include "stats.hpp"
#include "shop.hpp"
#include "draw_manager.hpp"

void peon::init(int ptier)
{
    tier = clamp(ptier, 0, stats::peon_class_money_base.size());

    float money = stats::peon_class_money_base[tier];

    money = money + money * randf_s(-stats::peon_money_variation, stats::peon_money_variation);

    wallet = (int)money;
}

void peon::seek_random_item(shop& s)
{
    /*for(sellable* i : s.for_sale)
    {

    }*/

    if(currently_seeking)
        return;

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

        should_pathfind = true;
        pathfinding_destination = conv_implicit<vec2f>(s.sellable_to_tile(currently_seeking)) * (float)s.grid_dim + s.grid_dim/2.f;

        success = true;
    }
}

bool peon::within_purchase_distance_of_currently_seeking(shop& s)
{
    sellable* c = currently_seeking;

    vec2f tile_pos = conv_implicit<vec2f>(s.sellable_to_tile(c)) + 0.5f;

    vec2f world_pos = pos;

    vec2f grid_pos = world_pos / s.grid_dim;

    vec2f dist = tile_pos - grid_pos;

    float len = dist.length();

    //printf("buydist %f\n", len);

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

    should_pathfind = false;

    return true;
}

void peon::pathfind(shop& s, float dt_s)
{
    if(!should_pathfind)
        return;

    /*vec2i target_grid = s.sellable_to_tile(currently_seeking);

    vec2f dest = (vec2f){target_grid.x(), target_grid.y()} * (float)s.grid_dim;*/

    vec2f dest = pathfinding_destination;

    vec2f cur = pos;

    float md = stats::peon_move_speed * dt_s;

    float dist = (dest - cur).length();

    //if(dist < md)
    //    md = dist;

    if(dist < 0.1f)
    {
        should_pathfind = false;
        return;
    }

    vec2f dir = (dest - cur).norm();

    cur = cur + dir * md;

    pos = cur;
}

void peon::set_pathfind(vec2f p)
{
    if((p - pos).length() < 0.1f)
        return;

    should_pathfind = true;

    pathfinding_destination = p;
}

void peon::cancel_pathfind()
{
    should_pathfind = false;

    pathfinding_destination = {-1.f, -1.f};

    currently_seeking = nullptr;
}

bool peon::should_leave(shop& s)
{
    std::vector<sellable*> sells = s.get_purchasable_sellables_on_tables();

    for(auto& i : sells)
    {
        if(i->listed_price < wallet)
            return false;
    }

    return true;
}

bool peon::within_door(shop& s)
{
    vec2f door_pos = s.get_door_world_pos() + s.grid_dim/2.f;

    return (pos - door_pos).length() < 0.4f;
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
        }

        i->pathfind(s, draw_manage.get_frametime_s());

        if(!i->should_pathfind)
            i->seek_random_item(s);

        if(i->should_leave(s))
        {
            //printf("sl");

            i->set_pathfind(s.get_door_world_pos() + s.grid_dim/2.f);
        }
    }

    for(int i=0; i<peons.size(); i++)
    {
        peon* p = peons[i];

        if(p->should_leave(s) && p->within_door(s))
        {
            remove_peon(p);
            i--;
        }
    }
}

///draw tooltips when mousing over peons with imgui
///need to know their cash
///also pathfind them out the shop
void peon_manager::draw_peons(draw_manager& draw_manage)
{
    sf::Color col(128, 128, 255);
    sf::CircleShape shape;
    shape.setFillColor(col);

    float rad = 3.f;

    shape.setRadius(rad);
    shape.setOrigin(rad, rad);

    sf::Text txt;
    txt.setFont(draw_manage.font);
    txt.setCharacterSize(12);

    for(peon* p : peons)
    {
        vec2f pos = p->pos;

        shape.setPosition({pos.x(), pos.y()});

        draw_manage.window.draw(shape);

        std::string wallet_money = to_string_prec(p->wallet, 4);

        txt.setString(wallet_money.c_str());
        txt.setPosition(shape.getPosition());

        draw_manage.window.draw(txt);
    }
}

void peon_manager::check_peon_release_sellable(sellable* s)
{
    for(peon* i : peons)
    {
        if(i->currently_seeking == s)
        {
            i->cancel_pathfind();
        }
    }
}
