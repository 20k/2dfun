#ifndef SHARED_HPP_INCLUDED
#define SHARED_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <stdint.h>
#include <string>
#include <map>
#include <net/shared.hpp>
#include <vec/vec.hpp>
#include <sstream>
#include <iomanip>
#include <SFML/Graphics.hpp>

template <typename T>
std::string to_string_prec(const T& a_value, const int n = 6)
{
    std::ostringstream out;
    out << std::setprecision(n) << a_value;
    return out.str();
}

struct input_state
{
    bool last = false;
};

///has the button been pressed once, and only once
template<sf::Keyboard::Key k>
bool once()
{
    static bool last;

    sf::Keyboard key;

    if(key.isKeyPressed(k) && !last)
    {
        last = true;

        return true;
    }

    if(!key.isKeyPressed(k))
    {
        last = false;
    }

    return false;
}

template<sf::Mouse::Button b>
bool once()
{
    static bool last;

    sf::Mouse m;

    if(m.isButtonPressed(b) && !last)
    {
        last = true;

        return true;
    }

    if(!m.isButtonPressed(b))
    {
        last = false;
    }

    return false;
}

template<sf::Mouse::Button b>
bool once(input_state& st)
{
    sf::Mouse m;

    if(m.isButtonPressed(b) && !st.last)
    {
        st.last = true;

        return true;
    }

    if(!m.isButtonPressed(b))
    {
        st.last = false;
    }

    return false;
}

#endif // SHARED_HPP_INCLUDED
