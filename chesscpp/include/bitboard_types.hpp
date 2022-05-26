#ifndef BB_TYPES
#define BB_TYPES

#include <stdint.h>
#include <map>

typedef std::array<std::array<int, 8>, 8> Board;
typedef std::vector<std::string> UCIStrings;

const std::map<u_int8_t, char>
    piece_code_to_char = {{0, ' '}, {2, 'q'}, {3, 'b'}, {4, 'n'}, {5, 'r'}};

struct move
{
    uint8_t src;              // 6 b
    uint8_t dest;             // 6 b
    uint8_t capture;          // 3 b (actually less)
    uint8_t flag;             // 2 b (promotion)
    uint8_t ep_field;         // 6 b
    uint8_t prev_c;           // 4 b
    uint8_t castling;         // 2 b
    uint8_t prev_half_move_c; // 6b
    uint8_t ep;
    // 29b
};

inline move create_move(const u_int8_t src, const uint8_t dest, const uint8_t capture = 0, const uint8_t flag = 0, const uint8_t cast = 0, const uint8_t ep = 0, uint8_t ep_field = 0)
{
    struct move m;
    m.src = src; // both
    m.dest = dest; // both
    m.flag = flag; // promotion, both
    m.ep_field = ep_field; // both but with different use
    m.capture = capture; // stack/pop
    m.prev_c = 0; // stack/pop
    m.castling = cast; // both
    m.prev_half_move_c = 0; // stack/pop
    m.ep = ep; // if ep_field == 0 => m.ep = false
    return m;
}

#endif