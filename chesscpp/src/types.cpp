#include "types.hpp"

Position operator+(const Position &x, const Position &y)
{
    return std::make_pair(x.first + y.first, x.second + y.second);
}