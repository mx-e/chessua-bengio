#include "../include/bitboard.hpp"

uint8_t get_piece_type_of_field(const C_Board *board, int position_idx)
{
    uint8_t piece = 0;
    uint64_t mask = most_sig_bit >> position_idx;
    piece += pKing * (mask & board->pieces[b_kings]);
    piece += pQueen * (mask & board->pieces[b_queens]);
    piece += pBishop * (mask & board->pieces[b_bishops]);
    piece += pKnight * (mask & board->pieces[b_knights]);
    piece += pRook * (mask & board->pieces[b_rooks]);
    piece += pPawn * (mask & board->pieces[b_pawns]);
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
    std::cout << move_board << "\n";
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
        std::cout << m << "\n";
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

void C_Board::collect_pawn_moves_and_captures()
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
    for (knight_direction dir : all_knight_directions)
    {
        extract_captures_with_offset(this, get_knight_moves_and_attacks(turn, dir), legal_moves, -1. * turn * knight_idx_offsets.at(dir));
    }
}

inline void C_Board::collect_king_moves_and_captures()
{
    int king_idx = scan_board(get_king(turn)).front();
    uint64_t moves = king_moves[king_idx] & get_empty_or_enemy(turn);
    extract_captures_with_explicit_src(this, moves, legal_moves, king_idx);
}

inline void C_Board::collect_legal_moves()
{
    std::cout << "collecting pawn moves"
              << "\n";
    collect_pawn_moves_and_captures();
    std::cout << "collecting knight moves"
              << "\n";
    collect_knight_moves_and_captures();
    std::cout << "collecting king moves"
              << "\n";
    collect_king_moves_and_captures();
    std::cout << "done"
              << "\n";
}