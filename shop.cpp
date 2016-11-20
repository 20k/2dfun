#include "shop.hpp"
#include "stats.hpp"
#include "item.hpp"
#include "economics.hpp"
#include "draw_manager.hpp"
#include <SFML/Graphics.hpp>

void shop::init(item_manager* imanage, vec2i pdim, int pgrid_dim)
{
    item_manage = imanage;

    dim = pdim;

    grid_dim = pgrid_dim;

    for(int i=0; i<dim.x() * dim.y(); i++)
    {
        int x = i % dim.x();
        int y = i / dim.x();

        tile base;
        base.tile_type = tile_info::FLOOR;
        base.array_pos = {x, y};

        tiles.push_back(base);
    }

    money = stats::starting_cash;
}

sellable* shop::make_sellable(item* i, float price)
{
    sellable* sell = new sellable;

    sell->i = i;

    if(price < 0)
    {
        ///purely helper class, no reference to i
        economic_item e;
        e.load_from_item(i);

        price = e.value;
    }

    sell->listed_price = price;

    for_sale.push_back(sell);

    return sell;
}

void shop::remove_sellable(sellable* s)
{
    for(int i=0; i<for_sale.size(); i++)
    {
        if(for_sale[i] == s)
        {
            for_sale.erase(for_sale.begin() + i);

            delete s;

            i--;
        }
    }
}

void shop::place_sellable(sellable* s, vec2f pos)
{
    vec2i local = pos_to_grid_snapped(pos);

    if(local.x() < 0 || local.y() < 0 || local.x() >= dim.x() || local.y() >= dim.y())
        return;

}

void shop::place_selection(vec2f pos)
{
    vec2i local = pos_to_grid_snapped(pos) / grid_dim;

    if(local.x() < 0 || local.y() < 0 || local.x() >= dim.x() || local.y() >= dim.y())
        return;

    tile& t = tiles[local.y() * dim.x() + local.x()];

    t.tile_type = tile_info::TABLE_WITH_ITEM;

    t.item_class = currently_placing.item_class;
    t.rarity = currently_placing.rarity;
    t.specific_object = currently_placing.specific_object;
}

vec2i shop::pos_to_grid_snapped(vec2f pos)
{
    pos = pos / (float)grid_dim;

    ///pos is an integer type
    pos = floor(pos);

    pos = pos * grid_dim;

    return conv_implicit<vec2i>(pos);
}

void shop::draw(draw_manager& draw_manage)
{
    if(!view_is_init)
    {
        draw_manage.view.setCenter(dim.x()/2.f - grid_dim/4.f, dim.y()/2.f - grid_dim/4.f);

        view_is_init = true;
    }

    draw_manage.window.setView(draw_manage.view);

    sf::RectangleShape shape;

    shape.setSize({grid_dim-1, grid_dim-1});

    sf::Color table_col = sf::Color(139,69,19);
    sf::Color floor_col = sf::Color(40,40,40);
    sf::Color wall_col = sf::Color(140, 128, 120);

    shape.setFillColor(floor_col);

    for(int jj=0; jj<dim.v[1] / grid_dim; jj++)
    {
        for(int ii=0; ii<dim.v[0] / grid_dim; ii++)
        {
            shape.setPosition(ii * grid_dim, jj * grid_dim);

            draw_manage.window.draw(shape);
        }
    }

    shape.setFillColor(table_col);

    sf::Text txt;
    txt.setFont(draw_manage.font);
    txt.setCharacterSize(10);

    ///draw tables here
    for(int jj=0; jj<dim.y() / grid_dim; jj++)
    {
        for(int ii=0; ii<dim.x() / grid_dim; ii++)
        {
            tile& t = tiles[jj*dim.x() + ii];

            std::string& str = t.item_class;

            if(t.tile_type == tile_info::TABLE_WITH_ITEM)
            {
                shape.setPosition(ii * grid_dim, jj * grid_dim);

                draw_manage.window.draw(shape);

                txt.setString(str.c_str());

                txt.setPosition(ii * grid_dim, jj * grid_dim);

                draw_manage.window.draw(txt);
            }
        }
    }

    vec2f mouse_pos = draw_manage.get_mouse_pos();

    vec2f local_pos = draw_manage.screen_to_world(mouse_pos);

    vec2i local_grid = pos_to_grid_snapped(local_pos);

    if(!currently_placing.placing)
        shape.setFillColor(sf::Color(76, 153, 0));
    else
    {
        shape.setFillColor(table_col);
    }

    shape.setPosition(local_grid.v[0], local_grid.v[1]);

    draw_manage.window.draw(shape);
}

struct sorted_item_info
{
    std::string item_class;

    std::vector<sellable*> items;

    float get_listed_price()
    {
        float v = 0.f;

        for(auto& i : items)
        {
            v += i->listed_price;
        }

        return v;
    }
};

std::vector<sorted_item_info> sort_to_class(const std::vector<sellable*>& items)
{
    std::map<std::string, sorted_item_info> rmap;

    for(auto& i : items)
    {
        rmap[i->i->item_class].item_class = i->i->item_class;

        rmap[i->i->item_class].items.push_back(i);
    }

    std::vector<sorted_item_info> ret;

    for(auto& i : rmap)
    {
        ret.push_back(i.second);
    }

    return ret;
}

void draw_expanded_rarity(draw_manager& draw_manage, shop& s, int rarity)
{
    std::vector<sellable*> items = s.get_sellable_by_rarity(rarity);

    std::vector<sorted_item_info> grouped_info = sort_to_class(items);

    ImGui::Indent();
    ImGui::BeginGroup();

    int cgrp = 0;

    for(auto& i : grouped_info)
    {
        std::string plural = "";

        if(i.items.size() > 1 && i.item_class[i.item_class.size()-1] != 'S')
            plural = "S";

        std::string item_class_header = i.item_class + plural + " (" + std::to_string(i.items.size()) + ")";

        item_class_header += " " + to_string_prec(i.get_listed_price(), 5) + " Gold";

        std::string bname = std::string("Place") + "##" + std::to_string(cgrp);

        if(ImGui::Button(bname.c_str()))
        {
            s.currently_placing.item_class = i.item_class;
            s.currently_placing.placing = true;
            s.currently_placing.rarity = rarity;
            s.currently_placing.specific_object = nullptr;
        }

        ImGui::SameLine();

        if(ImGui::CollapsingHeader(item_class_header.c_str()))
        {
            ImGui::Indent();
            ImGui::BeginGroup();

            int id = 0;

            for(sellable* j : i.items)
            {
                std::string str_id = std::to_string(id);

                item* it = j->i;

                std::string price = to_string_prec(j->listed_price, 5) + " Gold";

                if(ImGui::CollapsingHeader((it->item_class + " " + str_id + " " + price).c_str()))
                {

                }

                id++;
            }

            ImGui::EndGroup();
            ImGui::Unindent();
        }

        cgrp++;
    }

    ImGui::EndGroup();
    ImGui::Unindent();
}

void shop::draw_shopfront_ui(draw_manager& draw_manage)
{
    /*Uncommon LUTE of +1 INT +1 WIS
    Dealing an extra 0.000964 hp of damage
    Condition: PRISTINE
    Notoriety: GOOD
    39 XP/40*/

    ImGui::Begin("Shop Items");

    for(int i=0; i<stats::item_rarity.size(); i++)
    {
        std::string rarity = stats::item_rarity[i];

        if(get_sellable_by_rarity(i).size() > 0 && ImGui::CollapsingHeader(rarity.c_str()))
        {
            draw_expanded_rarity(draw_manage, *this, i);
        }
    }

    ImGui::End();
}

std::vector<sellable*> shop::get_sellable_by_rarity(int rarity)
{
    std::vector<sellable*> ret;

    for(auto& i : for_sale)
    {
        if(i->i->rarity == rarity)
            ret.push_back(i);
    }

    return ret;
}

void shop::tick(draw_manager& draw_manage)
{
    sf::Mouse mouse;

    //middle click to remove tiles?
    if(once<sf::Mouse::Right>())
    {
        currently_placing.placing = false;
    }

    if(once<sf::Mouse::Left>() && currently_placing.placing)
    {
        vec2f proj = draw_manage.get_mouse_pos();

        proj = draw_manage.screen_to_world(proj);

        place_selection(proj);
    }
}

vec2i shop::sellable_to_tile(sellable* s)
{
    for(auto& i : tiles)
    {
        if(i.specific_object == s->i)
            return i.array_pos;
    }

    printf("No sellable at any tile\n");

    return {-1, -1};
}

void shop::purchase(sellable* s)
{
    for(int i=0; i<for_sale.size(); i++)
    {
        if(for_sale[i] == s)
        {
            money += s->listed_price;

            item_manage->destroy(s->i);

            remove_sellable(s);

            i--;
        }
    }
}
