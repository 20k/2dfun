#include "peon_manager.hpp"
#include "stats.hpp"
#include "shop.hpp"
#include "draw_manager.hpp"
#include "item.hpp"

void peon::init(int ptier)
{
    tier = clamp(ptier, 0, stats::peon_class_money_base.size());

    float money = stats::peon_class_money_base[tier];

    money = money + money * randf_s(-stats::peon_money_variation, stats::peon_money_variation);

    wallet = (int)money;

    buy_threshold = randf_s(0.f, 1.f);
}

#if 0
void peon::seek_random_item(shop& s)
{
    /*for(sellable* i : s.for_sale)
    {

    }*/

    ///so eventually we can add to the buy threshold
    ///based on local environmental modifiers, how many nice well stocked stalls we've visited
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
    int max_tries = 40;

    while(!success && c < max_tries)
    {
        c++;

        int rseek = randf<1, int>(0, (int)purchaseable_sellables.size());

        sellable* seek = purchaseable_sellables[rseek];

        if(seek->listed_price >= wallet || seek->locked)
            continue;

        if(!might_buy(seek))
            continue;

        seek->locked = true;

        vec2f dest_pos = conv_implicit<vec2f>(s.sellable_to_tile(seek)) * (float)s.grid_dim + s.grid_dim/2.f;

        command_element next_command;
        next_command.currently_seeking = seek;
        next_command.command = peon_command::SEEK;
        next_command.pathfinding_destination = dest_pos;

        command_list.push_back(next_command);

        success = true;
    }
}


bool peon::is_currently_seeking(sellable* s)
{
    //return s == currently_seeking && should_pathfind;

    if(no_commands())
        return false;

    if(!is_command_type(peon_command::SEEK))
        return false;

    return s == get_current_command().currently_seeking;
}

void peon::set_pathfind(vec2f p)
{
    if((p - pos).length() < 0.1f)
        return;

    command_element ce;
    ce.command = peon_command::SEEK;
    ce.pathfinding_destination = p;

    command_list.push_back(ce);

    //should_pathfind = true;

    //pathfinding_destination = p;
}

void peon::cancel_pathfind()
{
    /*should_pathfind = false;

    pathfinding_destination = {-1.f, -1.f};

    currently_seeking = nullptr;*/

    pop_front_command();
}

///change this to be "am i done", not "is there anything left"
bool peon::should_leave(shop& s)
{
    std::vector<sellable*> sells = s.get_purchasable_sellables_on_tables();

    ///and should not be forced to leave shop due to closure
    if(time_since_spawn_s < stats::peon_idle_time_total_s)
        return false;

    ///if we're pathfind but we're not pathfinding to the exit
    //if(should_pathfind && !currently_leaving)
    //return false;

    if(get_current_command().command !=  )

    for(auto& i : sells)
    {
        if(is_currently_seeking(i))
            return false;

        if(i->listed_price < wallet && !i->locked && might_buy(i))
            return false;
    }

    return true;
}

///idle if there's nothing I can buy, for a time?
bool peon::should_idle(shop& s)
{
    return !should_pathfind && !should_leave(s) && idling_time_s > stats::peon_idle_time_loiter_s;
}

void peon::idle_pathfind(shop& s)
{
    vec2f dest = randv<2, float>((vec2f){10.f, 10.f}, conv_implicit<vec2f>(s.dim) - 10.f);

    //printf("dst %f %f\n", EXPAND_2(dest));

    set_pathfind(dest);

    idling_time_s = 0.f;
}

void peon::table_display_pathfind(shop& s)
{
    vec2i random_table = get_random_table_display(s);

    ///this is really a failure case, but not 100% sure i actually want to throw
    ///because its fairly benign, but then you know, fail early
    if(random_table.v[0] == -1)
    {
        printf("0 tile size in display pathfind");
        return;
    }

    float rangle = randf_s(0.f, M_PI * 2.f);

    vec2f rpos = {cos(rangle) * s.grid_dim, sin(rangle) * s.grid_dim};

    vec2f real_pos = (conv_implicit<vec2f>(random_table) + 0.5f) * (float)s.grid_dim;

    set_pathfind(real_pos + rpos);
}
#endif

vec2f peon::get_idle_position(shop& s)
{
    vec2f dest = randv<2, float>((vec2f){10.f, 10.f}, conv_implicit<vec2f>(s.dim) - 10.f);

    return dest;
}

bool peon::might_buy(sellable* s)
{
    float wallet_frac = s->listed_price / wallet;

    ///due to price
    float willingness_removed = buy_threshold * wallet_frac * stats::max_price_reluctance_frac;

    return s->listed_price < wallet && (buy_threshold + willingness_removed) < stats::raw_buy_threshold;
}

bool peon::within_purchase_distance_of_currently_seeking(shop& s)
{
    if(no_commands())
        return false;

    command_element cur = get_current_command();

    sellable* c = cur.currently_seeking;

    vec2f tile_pos = conv_implicit<vec2f>(s.sellable_to_tile(c)) + 0.5f;

    vec2f world_pos = pos;

    vec2f grid_pos = world_pos / (float)s.grid_dim;

    vec2f dist = tile_pos - grid_pos;

    float len = dist.length();

    //printf("buydist %f\n", len);

    if(len < 0.5)
        return true;

    return false;
}

bool peon::can_purchase_currently_seeking(shop& s)
{
    if(no_commands())
        return false;

    bool is_within_dist = within_purchase_distance_of_currently_seeking(s);

    float price = get_current_command().currently_seeking->listed_price;

    if(is_within_dist && price < wallet)
    {
        return true;
    }

    return false;
}


bool peon::try_purchase_currently_seeking(shop& s)
{
    if(!can_purchase_currently_seeking(s))
        return false;

    sellable* sell = get_current_command().currently_seeking;

    wallet -= sell->listed_price;

    ///add to peon invent?
    s.purchase(sell);

    return true;
}

vec2i peon::get_random_table_display(shop& s)
{
    std::vector<tile> tiles = s.get_table_tiles();

    if(tiles.size() == 0)
        return {-1, -1};

    int v = randf<1, int>(0, tiles.size());

    return tiles[v].array_pos;
}

bool sellable_on_table(sellable* s, tile t)
{
    if((t.item_class == s->i->item_class && t.rarity == s->i->rarity) || s->i == t.specific_object)
    {
        return true;
    }

    return false;
}

vec2f tile_to_pos(vec2i array_pos, shop& s)
{
    return conv_implicit<vec2f>(array_pos) * (float)s.grid_dim + s.grid_dim/2.f;;
}

sellable* peon::get_random_item_at_table(vec2i tab, shop& s)
{
    tile t = s.tiles[tab.y() * s.dim.x() / s.grid_dim + tab.x()];

    std::vector<sellable*> purchaseable_sellables = s.get_purchasable_sellables_on_tables();

    if(purchaseable_sellables.size() == 0)
        return nullptr;

    std::vector<sellable*> valid_sellables;

    for(sellable* s : purchaseable_sellables)
    {
        if(s->locked)
            continue;

        if(!might_buy(s))
            continue;

        if(sellable_on_table(s, t))
            valid_sellables.push_back(s);
    }

    if(valid_sellables.size() == 0)
        return nullptr;

    int random_sell = randf<1, int>(0, valid_sellables.size());

    return valid_sellables[random_sell];
}

bool peon::pathfind(shop& s, float dt_s, float cancel_dist)
{
    if(!is_command_type(peon_command::SEEK))
        return false;

    command_element ce = get_current_command();

    vec2f dest = ce.pathfinding_destination;

    vec2f cur = pos;

    float md = stats::peon_move_speed * dt_s;

    float dist = (dest - cur).length();

    if(dist < cancel_dist)
    {
        return true;
    }

    vec2f dir = (dest - cur).norm();

    cur = cur + dir * md;

    pos = cur;

    idling_time_s = 0.f;

    return false;
}

///peon logic
///wander around idle
///wander over to look at tables
///probability to buy something

///only remaining thing is to leave the shop after a certain amount of time
void peon::tick(shop& s, draw_manager& draw_manage)
{
    command_element current = get_current_command();

    if(current.command == peon_command::NONE)
    {
        vec2i random_table = get_random_table_display(s);

        float idle_chance = 0.5f;

        float rand_n = randf_s(0.f, 1.f);

        if(random_table.x() != -1 && rand_n >= idle_chance)
        {
            command_element ce;
            ce.command = peon_command::SEEK;
            ce.pathfinding_destination = tile_to_pos(random_table, s);
            ce.currently_seeking = get_random_item_at_table(random_table, s);
            ce.cancel_dist = s.grid_dim/2.f;

            if(ce.currently_seeking != nullptr)
                ce.currently_seeking->locked = true;

            command_list.push_back(ce);

            ce.command = peon_command::RESET_IDLE_TIME;

            command_list.push_back(ce);

            ce.command = peon_command::WAIT;
            ce.dur_s = 4.f;

            command_list.push_back(ce);

            ce.command = peon_command::POTENTIALLY_PURCHASE;
            ce.dur_s = 0.f;

            command_list.push_back(ce);
        }
        else
        {
            command_element ce;
            ce.command = peon_command::SEEK;
            ce.pathfinding_destination = get_idle_position(s);

            command_list.push_back(ce);

            ce.command = peon_command::RESET_IDLE_TIME;

            command_list.push_back(ce);

            ce.command = peon_command::WAIT;
            ce.dur_s = 2.f;

            command_list.push_back(ce);
        }
    }

    if(current.command == peon_command::SEEK)
    {
        bool should_pop = pathfind(s, draw_manage.get_frametime_s(), get_current_command().cancel_dist);

        if(should_pop)
        {
            pop_front_command();
        }
    }

    if(current.command == peon_command::POTENTIALLY_PURCHASE)
    {
        if(get_current_command().currently_seeking != nullptr)
        {
            try_purchase_currently_seeking(s);
        }

        pop_front_command();
    }

    if(current.command == peon_command::WAIT)
    {
        if(idling_time_s >= current.dur_s)
            pop_front_command();
    }

    if(current.command == peon_command::RESET_IDLE_TIME)
    {
        idling_time_s = 0.f;

        pop_front_command();
    }

    if(time_since_spawn_s >= stats::peon_idle_time_total_s)
    {
        command_element ce;
        ce.command = peon_command::SEEK;
        ce.pathfinding_destination = s.get_door_world_pos() + s.grid_dim/2.f;
        ce.cancel_dist = s.grid_dim/4.f;

        command_list.push_back(ce);

        ce.command = peon_command::LEAVE;

        command_list.push_back(ce);
    }

    if(current.command == peon_command::LEAVE)
    {
        kill_me = true;
    }

    time_since_spawn_s += draw_manage.get_frametime_s();

    idling_time_s += draw_manage.get_frametime_s();
}

bool peon::within_door(shop& s)
{
    vec2f door_pos = s.get_door_world_pos() + s.grid_dim/2.f;

    return (pos - door_pos).length() < s.grid_dim/3.f;
}

void peon::force_unseek(sellable* s)
{
    if(s == nullptr)
        return;

    for(int i=0; i<command_list.size(); i++)
    {
        if(command_list[i].currently_seeking == s)
        {
            command_list.erase(command_list.begin() + i);
            i--;
        }
    }
}

/*void peon::force_unseek(sellable* s)
{
    if(s == nullptr)
        return;

    if(s == currently_seeking)
    {
        cancel_pathfind();
    }
}*/

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
    /*std::vector<tile> display_table_tiles = s.get_table_tiles();

    for(peon* i : peons)
    {
        if(i->currently_seeking)
        {
            bool success = i->try_purchase_currently_seeking(s);
        }

        i->pathfind(s, draw_manage.get_frametime_s());

        ///need to make it random chance as to whether they're going to buy something now
        if(!i->should_pathfind)
            i->seek_random_item(s);

        if(i->should_leave(s))
        {
            //printf("sl");

            i->set_pathfind(s.get_door_world_pos() + s.grid_dim/2.f);

            i->currently_leaving = true;
        }

        if(i->should_idle(s))
        {
            if(randf_s(0.f, 1.f) < 0.3f || display_table_tiles.size() == 0)
                i->idle_pathfind(s);
            else
            {
                i->table_display_pathfind(s);
            }
        }

        i->time_since_spawn_s += draw_manage.get_frametime_s();

        i->idling_time_s += draw_manage.get_frametime_s();
    }

    for(int i=0; i<peons.size(); i++)
    {
        peon* p = peons[i];

        if(p->should_leave(s) && p->within_door(s))
        {
            remove_peon(p);
            i--;
        }
    }*/

    for(peon* i : peons)
    {
        i->tick(s, draw_manage);
    }

    for(int i=0; i<peons.size(); i++)
    {
        peon* p = peons[i];

        if(p->kill_me)
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

void peon_manager::force_unseek(sellable* s)
{
    for(auto& i : peons)
    {
        i->force_unseek(s);
    }
}

command_element peon::get_current_command()
{
    command_element none_command;
    none_command.command = peon_command::NONE;

    if(command_list.size() == 0)
        return none_command;

    return command_list.front();
}

bool peon::is_command_type(peon_command::commands c)
{
    return get_current_command().command == c;
}

bool peon::no_commands()
{
    return get_current_command().command == peon_command::NONE;
}

void peon::pop_front_command()
{
    if(command_list.size() == 0)
        return;

    command_list.erase(command_list.begin() + 0);
}

bool peon::is_leaving()
{
    return get_current_command().command == peon_command::LEAVE;
}

bool peon::will_leave()
{
    for(auto& i : command_list)
    {
        if(i.command == peon_command::LEAVE)
            return true;
    }

    return false;
}
