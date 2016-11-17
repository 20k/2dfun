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

template <typename T>
std::string to_string_prec(const T& a_value, const int n = 6)
{
    std::ostringstream out;
    out << std::setprecision(n) << a_value;
    return out.str();
}

#endif // SHARED_HPP_INCLUDED
