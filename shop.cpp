#include "shop.hpp"
#include "stats.hpp"
#include "item.hpp"
#include "economics.hpp"
#include "draw_manager.hpp"

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

vec2i shop::pos_to_grid_snapped(vec2i pos)
{
    pos = pos / (float)grid_dim;

    ///pos is an integer type
    //pos = floor(pos);

    pos = pos * grid_dim;

    return pos;
}

void shop::draw(draw_manager& draw_manage)
{

}
