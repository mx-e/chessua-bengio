#include "../include/bitboard.hpp"

uint8_t get_piece_type_of_field(const C_Board *board, int position_idx)
{
    uint8_t piece = 0;
    uint64_t mask = most_sig_bit >> position_idx;
    piece += pKing * (bool)(mask & board->pieces[b_kings]);
    piece += pQueen * (bool)(mask & board->pieces[b_queens]);
    piece += pBishop * (bool)(mask & board->pieces[b_bishops]);
    piece += pKnight * (bool)(mask & board->pieces[b_knights]);
    piece += pRook * (bool)(mask & board->pieces[b_rooks]);
    piece += pPawn * (bool)(mask & board->pieces[b_pawns]);
    return piece;
}

void extract_promotions(uint64_t move_board, std::vector<move> &move_list)
{
    if (move_board == 0)
        return;
    {
        for (int m : scan_board(move_board))
        {
            for (uint8_t p : {2, 3, 4, 5})
            {
                move_list.push_back(create_move((uint8_t)m + 1, (uint8_t)m, 0, p));
            }
        }
    }
}

void extract_promotion_captures(const C_Board *board, uint64_t move_board, std::vector<move> &move_list)
{
    if (move_board == 0)
        return;
    {
        for (int m : scan_board(move_board))
        {
            for (uint8_t p : {2, 3, 4, 5})
            {
                move_list.push_back(create_move((uint8_t)m + 1, (uint8_t)m, get_piece_type_of_field(board, m), p));
            }
        }
    }
}

void extract_moves_with_offset(uint64_t move_board, std::vector<move> &move_list, int source_offset)
{
    if (move_board == 0)
        return;
    for (int m : scan_board(move_board))
    {
        move_list.push_back(create_move(m + source_offset, (uint8_t)m));
    }
}

void extract_captures_with_offset(const C_Board *board, uint64_t move_board, std::vector<move> &move_list, int source_offset)
{
    if (move_board == 0)
        return;
    for (int m : scan_board(move_board))
    {
        move_list.push_back(create_move(m + source_offset, m, get_piece_type_of_field(board, (uint8_t)m)));
    }
}

void extract_moves_with_explicit_src(uint64_t move_board, std::vector<move> &move_list, int src_idx)
{
    if (move_board == 0)
        return;
    for (int m : scan_board(move_board))
    {
        move_list.push_back(create_move(src_idx, (uint8_t)m));
    }
}

void extract_captures_with_explicit_src(const C_Board *board, uint64_t move_board, std::vector<move> &move_list, int src_idx)
{
    if (move_board == 0)
        return;
    for (int m : scan_board(move_board))
    {
        move_list.push_back(create_move((uint8_t)src_idx, (uint8_t)m, get_piece_type_of_field(board, m)));
    }
}

UCIStrings get_uci_moves(C_Board board)
{
    board.collect_legal_moves();
    std::vector<move> moves = board.legal_moves;
    UCIStrings uci_moves = {};
    for (move m : moves)
    {
        uci_moves.push_back(move_to_uci_str(m));
    }
    return uci_moves;
}

inline void C_Board::collect_pawn_moves_and_captures()
{
    extract_moves_with_offset(get_pawn_single_moves(turn), legal_moves, -1. * turn);
    extract_promotions(get_pawn_promotions(turn), legal_moves);
    extract_promotion_captures(this, get_pawn_promotion_attacks_left(turn), legal_moves);
    extract_promotion_captures(this, get_pawn_promotion_attacks_right(turn), legal_moves);
    extract_moves_with_offset(get_pawn_double_moves(turn), legal_moves, -2 * turn);
    extract_captures_with_offset(this, get_pawn_attacks_left(turn), legal_moves, -9 * turn);
    extract_captures_with_offset(this, get_pawn_attacks_right(turn), legal_moves, 7 * turn);
}

inline void C_Board::collect_knight_moves_and_captures()
{
    for (int knight_idx : scan_board(get_knights(turn)))
    {
        extract_moves_with_explicit_src(get_knight_moves(turn, knight_idx), legal_moves, knight_idx);
        extract_captures_with_explicit_src(this, get_knight_attacks(turn, knight_idx), legal_moves, knight_idx);
    }
}

inline void C_Board::collect_king_moves_and_captures()
{
    int king_idx = scan_board(get_king(turn)).front();
    uint64_t moves = king_moves[king_idx] & get_empty_or_enemy(turn);
    extract_captures_with_explicit_src(this, moves, legal_moves, king_idx);
}

inline void C_Board::collect_bishop_moves_and_captures()
{
    for (int bishop_idx : scan_board(get_bishops(turn)))
    {
        uint64_t bishop_moves = get_bishop_moves_and_attacks(bishop_idx, get_all_pieces());
        extract_moves_with_explicit_src(bishop_moves & get_empty_fields(), legal_moves, bishop_idx);
        extract_captures_with_explicit_src(this, bishop_moves & get_enemy_fields(turn), legal_moves, bishop_idx);
    }
}

inline void C_Board::collect_rook_moves_and_captures()
{
    for (int rook_idx : scan_board(get_rooks(turn)))
    {
        uint64_t rook_moves = get_rook_moves_and_attacks(rook_idx, get_all_pieces());
        extract_moves_with_explicit_src(rook_moves & get_empty_fields(), legal_moves, rook_idx);
        extract_captures_with_explicit_src(this, rook_moves & get_enemy_fields(turn), legal_moves, rook_idx);
    }
}

inline void C_Board::collect_queen_moves_and_captures()
{
    for (int queen_idx : scan_board(get_queen(turn)))
    {
        uint64_t queen_moves = get_bishop_moves_and_attacks(queen_idx, get_all_pieces()) | get_rook_moves_and_attacks(queen_idx, get_all_pieces());
        extract_moves_with_explicit_src(queen_moves & get_empty_fields(), legal_moves, queen_idx);
        extract_captures_with_explicit_src(this, queen_moves & get_enemy_fields(turn), legal_moves, queen_idx);
    }
}

void C_Board::collect_legal_moves()
{
    collect_pawn_moves_and_captures();
    collect_knight_moves_and_captures();
    collect_king_moves_and_captures();
    collect_bishop_moves_and_captures();
    collect_queen_moves_and_captures();
    collect_rook_moves_and_captures();
}

void C_Board::push_move(move m)
{
    // update board
    uint8_t piece_type = get_piece_type_of_field(this, m.src);
    unset_single_piece(turn, piece_type, m.src);
    if (m.capture)
    {
        unset_single_piece(turn * -1, m.capture, m.dest);
    }
    if (m.flag)
    {
        piece_type = m.flag;
    }
    set_single_piece(turn, piece_type, m.dest);

    // set counters
    m.prev_half_move_c = half_moves;
    half_moves += 1.;
    half_moves *= (!(piece_type == pPawn) && !(m.capture));
    moves += (turn == -1.) * 1.;

    turn *= -1;

    move_stack.push(m);
}

move C_Board::pop_move()
{
    if (move_stack.empty())
    {
        return create_move(0, 0);
    }
    move m = move_stack.top();

    // update board
    uint8_t piece_type = get_piece_type_of_field(this, m.dest);
    unset_single_piece(turn * -1, piece_type, m.dest);
    if (m.capture)
    {
        set_single_piece(turn, m.capture, m.dest);
    }
    if (m.flag)
    {
        piece_type = pPawn;
    }
    set_single_piece(turn * -1, piece_type, m.src);

    // set_counters
    half_moves = m.prev_half_move_c;
    moves -= (turn == 1.) * 1.;

    turn *= -1;
    move_stack.pop();
    return m;
}