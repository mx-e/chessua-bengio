#ifndef TYPES
#define TYPES

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
    uint8_t promotion;             // 2 b (promotion)
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
    m.src = src; // both
    m.dest = dest; // both
    m.promotion = promotion; // promotion, both
    m.ep_field = ep_field; // both but with different use
    m.capture = capture; // stack/pop
    m.prev_c = 0; // stack/pop
    m.castling = cast; // both
    m.prev_half_move_c = 0; // stack/pop
    m.ep = ep; // if ep_field == 0 => m.ep = false
    return m;
}

struct C_BoardState
{
    uint64_t pieces[8];
    uint8_t castling_rights;
    uint8_t en_passant;
    std::vector<move> move_stack;
    std::vector<int> idx_list;
    float turn;
    float moves;
    float half_moves;
    bool king_attack;
    bool castling_move_illegal;
};

typedef std::vector<move> MoveList;
typedef std::vector<MoveList> MoveListStack;

struct C_Session
{
    MoveListStack move_list_stack;
    C_BoardState board_state;
};

inline void reserve_move_list_stack(MoveListStack &move_list_stack)
{
    move_list_stack.reserve(12);
    for (auto move_list : move_list_stack)
    {
        move_list.reserve(10);
    }
}

inline void reserve_board_state(C_BoardState &board_state)
{
    board_state.move_stack.reserve(10);
    board_state.idx_list.reserve(16);
}

inline C_Session construct_session()
{
    C_Session session{
        .board_state = C_BoardState{
            .castling_rights = UINT8_C(0x0F),
            .en_passant = UINT8_C(0x00),
            .turn = 1.,
            .moves = 0.,
            .half_moves = 0.,
            .king_attack = false,
            .castling_move_illegal = false}};

    reserve_move_list_stack(session.move_list_stack);
    reserve_board_state(session.board_state);    

    return session;
}


#endif