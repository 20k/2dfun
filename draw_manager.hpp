#ifndef DRAW_MANAGER_HPP_INCLUDED
#define DRAW_MANAGER_HPP_INCLUDED

#include <SFML/Graphics.hpp>

struct draw_manager
{
    sf::RenderWindow window;
    sf::View view;

    void init(int width, int height)
    {
        window.create(sf::VideoMode(800, 600), "Title");

        view = window.getDefaultView();
    }

    void tick()
    {
        window.setView(view);

        sf::Event Event;

        while(window.pollEvent(Event))
        {
            //ImGui::SFML::ProcessEvent(Event);

            if(Event.type == sf::Event::Closed)
            {
                window.close();
            }

            /*if(Event.type == sf::Event::Resized)
            {
                do_resize = true;

                r_x = Event.size.width;
                r_y = Event.size.height;

                s.width = r_x;
                s.height = r_y;

                s.save("./res/settings.txt");
            }

            if(Event.type == sf::Event::MouseWheelScrolled)
            {
                window.update_scrollwheel_delta(Event);
            }

            if(Event.type == sf::Event::GainedFocus)
            {
                window.set_focus(true);
            }

            if(Event.type == sf::Event::LostFocus)
            {
                window.set_focus(false);
            }*/
        }
    }

    void flip()
    {
        window.display();
        window.clear();
    }
};

#endif // DRAW_MANAGER_HPP_INCLUDED
