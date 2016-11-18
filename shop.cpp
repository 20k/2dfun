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

    shape.setFillColor(floor_col);

    for(int jj=0; jj<dim.v[1] / grid_dim; jj++)
    {
        for(int ii=0; ii<dim.v[0] / grid_dim; ii++)
        {
            shape.setPosition(ii * grid_dim, jj * grid_dim);

            draw_manage.window.draw(shape);
        }
    }

    ///draw tables here


    vec2f mouse_pos = draw_manage.get_mouse_pos();

    vec2f local_pos = draw_manage.screen_to_world(mouse_pos);

    vec2i local_grid = pos_to_grid_snapped(local_pos);

    shape.setFillColor(sf::Color(76, 153, 0));

    shape.setPosition(local_grid.v[0], local_grid.v[1]);

    draw_manage.window.draw(shape);
}
