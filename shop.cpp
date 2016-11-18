#include "shop.hpp"
#include "stats.hpp"
#include "item.hpp"
#include "economics.hpp"

void shop::init(item_manager* imanage)
{
    item_manage = imanage;
}

sellable* shop::make_sellable(item* i, float price)
{
    sellable* sell = new sellable;

    sell->i = i;

    if(price < 0)
    {
        economic_item e;
        e.load_from_item(i);

        price = e.value;
    }

    sell->listed_price = price;

    for_sale.push_back(sell);

    return sell;
}
