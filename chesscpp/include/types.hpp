#ifndef TYPES
#define TYPES

#include <stdint.h>
#include <map>
<<<<<<< HEAD
#include <string>
#include <tuple>
=======
#include <random>
>>>>>>> 7a93789353af307032be4060f45b0210b4ebd6d8
#include "constants.hpp"

typedef std::array<std::array<int, 8>, 8> Board;
typedef std::vector<std::string> UCIStrings;
typedef std::tuple<std::string, uint32_t> RunInfo;

const std::map<u_int8_t, char>
    piece_code_to_char = {{0, ' '}, {2, 'q'}, {3, 'b'}, {4, 'n'}, {5, 'r'}};

struct move
{
    uint8_t src;              // 6 b
    uint8_t dest;             // 6 b
    uint8_t capture;          // 3 b (actually less)
    uint8_t promotion;        // 2 b (promotion)
    uint8_t ep_field;         // 6 b
    uint8_t prev_c;           // 4 b
    uint8_t castling;         // 2 b
    uint8_t prev_half_move_c; // 6b
    uint8_t ep;
    // 29b
};

inline move create_move(const u_int8_t src, const uint8_t dest, const uint8_t capture = 0, const uint8_t promotion = 0, const uint8_t cast = 0, const uint8_t ep = 0, uint8_t ep_field = 0)
{
    struct move m;
    m.src = src;             // both
    m.dest = dest;           // both
    m.promotion = promotion; // promotion, both
    m.ep_field = ep_field;   // both but with different use
    m.capture = capture;     // stack/pop
    m.prev_c = 0;            // stack/pop
    m.castling = cast;       // both
    m.prev_half_move_c = 0;  // stack/pop
    m.ep = ep;               // if ep_field == 0 => m.ep = false
    return m;
}

inline move create_empty_move()
{
    return create_move(0, 0);
}

inline bool is_move_empty(move m)
{
    return m.src == m.dest;
}

typedef std::vector<move>
    MoveList;

struct C_BoardState
{
    uint64_t pieces[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t castling_rights = UINT8_C(0x0F);
    uint8_t en_passant = UINT8_C(0x00);
    uint64_t all_attacks = empty_board;
    MoveList move_stack;
    float turn = 1.;
    float moves = 0.;
    float half_moves = 0.;
    bool king_attack = false;
    bool castling_move_illegal = false;
};

struct Line
{
    int n_moves = 0;        // Number of moves in the line.
    move argmove[25] = {0}; // The line.
};

struct AlphaBetaState
{
    int max_depth;
    int current_max_depth = 1;
    float current_alpha = -infty;
    float current_beta = infty;
    move bestmove;
    std::vector<float> runtimes_at_depth = {};
    std::vector<uint32_t> nodes_at_depth = {};
    Line current_best_line = Line();
};

typedef std::vector<MoveList> MoveListStack;

#endif