#ifndef BB_TYPES
#define BB_TYPES

#include <stdint.h>
#include <map>

const std::map<u_int8_t, char>
    piece_code_to_char = {{0, ' '}, {2, 'q'}, {3, 'b'}, {4, 'n'}, {5, 'r'}};

struct move
{
    uint8_t src;
    uint8_t dest;
    uint8_t capture;
    uint8_t flag;
};

inline move create_move(const u_int8_t src, const uint8_t dest, const uint8_t capture = 0, const uint8_t flag = 0)
{
    struct move m;
    m.src = src;
    m.dest = dest;
    m.flag = flag;
    return m;
}

#endif