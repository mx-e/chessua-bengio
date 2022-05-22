#include "../include/bitboard.hpp"

inline uint8_t get_piece_type_of_field(const C_Board *board, int position_idx)
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

void extract_promotions(uint64_t move_board, std::vector<move> &move_list, float source_offset)
{
    if (move_board == 0)
        return;
    {
        for (int m : scan_board(move_board))
        {
            for (uint8_t p : {2, 3, 4, 5})
            {
                move_list.push_back(create_move(m + (int)source_offset, (uint8_t)m, 0, p));
            }
        }
    }
}

void extract_promotion_captures(const C_Board *board, uint64_t move_board, std::vector<move> &move_list, float source_offset)
{
    if (move_board == 0)
        return;
    {
        for (int m : scan_board(move_board))
        {
            for (uint8_t p : {2, 3, 4, 5})
            {
                move_list.push_back(create_move(m + (int)source_offset, (uint8_t)m, get_piece_type_of_field(board, m), p));
            }
        }
    }
}

void extract_moves_with_offset(uint64_t move_board, std::vector<move> &move_list, float source_offset)
{
    if (move_board == 0)
        return;
    for (int m : scan_board(move_board))
    {
        move_list.push_back(create_move(m + (int)source_offset, (uint8_t)m));
    }
}

void extract_pawn_double_moves(uint64_t move_board, std::vector<move> &move_list, float color)
{
    if (move_board == 0)
        return;
    for (int m : scan_board(move_board))
    {
        move_list.push_back(create_move(m - (int)color * 2., (uint8_t)m, 0, 0, 0, 0, m - (int)color));
    }
}

void extract_captures_with_offset(const C_Board *board, uint64_t move_board, std::vector<move> &move_list, float source_offset)
{
    if (move_board == 0)
        return;
    for (int m : scan_board(move_board))
    {
        move_list.push_back(create_move(m + (int)source_offset, m, get_piece_type_of_field(board, (uint8_t)m)));
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

void extract_ep_captures(const C_Board &board, uint64_t move_board, std::vector<move> &move_list, float source_offset)
{
    if (move_board == 0)
        return;
    for (int m : scan_board(move_board))
    {
        move_list.push_back(create_move(m + (int)source_offset, (uint8_t)m, pPawn, 0, 0, 1));
    }
}

UCIStrings get_uci_moves(C_Board &board)
{
    board.collect_legal_moves();
    std::vector<move> moves = board.legal_moves;
    moves.reserve(32);
    UCIStrings uci_moves = {};
    for (move m : moves)
    {
        if (check_move_causes_check(board, m))
        {
            continue;
        }
        uci_moves.push_back(move_to_uci_str(m));
    }
    return uci_moves;
}

inline uint64_t C_Board::collect_pawn_moves_and_captures()
{
    uint64_t promotion_row = turn == 1. ? row_8 : row_1;

    uint64_t pawn_single_moves = get_pawn_single_moves(turn);

    extract_moves_with_offset(pawn_single_moves & ~promotion_row, legal_moves, -pawn_shift_map.at(single) * turn);
    extract_promotions(pawn_single_moves & promotion_row, legal_moves, -pawn_shift_map.at(single) * turn);

    extract_pawn_double_moves(get_pawn_double_moves(pawn_single_moves, turn), legal_moves, turn);

    uint64_t pawn_attacks_left = get_pawn_attacks(attack_left, turn);
    uint64_t pawn_attacks_right = get_pawn_attacks(attack_right, turn);

    float left_offset = -pawn_shift_map.at(attack_left) * turn;
    float right_offset = -pawn_shift_map.at(attack_right) * turn;
    extract_captures_with_offset(this,
                                 pawn_attacks_left & (~promotion_row) & get_enemy_fields(turn),
                                 legal_moves,
                                 left_offset);
    extract_captures_with_offset(this,
                                 pawn_attacks_right & (~promotion_row) & get_enemy_fields(turn),
                                 legal_moves,
                                 right_offset);
    extract_promotion_captures(this,
                               pawn_attacks_left & promotion_row & get_enemy_fields(turn),
                               legal_moves,
                               left_offset);
    extract_promotion_captures(this,
                               pawn_attacks_right & promotion_row & get_enemy_fields(turn),
                               legal_moves,
                               right_offset);

    uint64_t ep_fields = set_board_1_at_idx(empty_board, en_passant) & ep_zone;
    extract_ep_captures(*this, ep_fields & pawn_attacks_left, legal_moves, left_offset);
    extract_ep_captures(*this, ep_fields & pawn_attacks_right, legal_moves, right_offset);

    return pawn_attacks_left | pawn_attacks_right;
}

uint64_t C_Board::collect_knight_moves_and_captures()
{
    uint64_t knight_attacks = empty_board;
    for (int knight_idx : scan_board(get_knights(turn)))
    {
        uint64_t knight_moves = get_knight_moves(knight_idx);
        knight_attacks |= knight_moves;
        extract_moves_with_explicit_src(knight_moves & get_empty_fields(), legal_moves, knight_idx);
        extract_captures_with_explicit_src(this, knight_moves & get_enemy_fields(turn), legal_moves, knight_idx);
    }
    return knight_attacks;
}

inline uint64_t C_Board::collect_king_moves_and_captures()
{
    uint64_t king = get_king(turn);
    if (!king)
        return 0;
    int king_idx = scan_board(king).front();
    uint64_t king_moves = get_king_moves(king_idx);
    extract_moves_with_explicit_src(king_moves & get_empty_fields(), legal_moves, king_idx);
    extract_captures_with_explicit_src(this, king_moves & get_enemy_fields(turn), legal_moves, king_idx);

    // castling_moves
    const std::array<castling, 2> castling_sides = color_to_castling_indicator.at(turn);
    if (get_castling_possible(castling_sides.at(0)))
    {
        legal_moves.push_back(create_move(king_idx, king_idx + 16, 0, 0, 1));
    }
    if (get_castling_possible(castling_sides.at(1)))
    {
        legal_moves.push_back(create_move(king_idx, king_idx - 16, 0, 0, 2));
    }
    return king_moves;
}

uint64_t C_Board::collect_bishop_moves_and_captures()
{
    uint64_t bishop_attacks = empty_board;
    for (int bishop_idx : scan_board(get_bishops(turn)))
    {
        uint64_t bishop_moves = get_bishop_moves_and_attacks(bishop_idx, get_all_pieces());
        bishop_attacks |= bishop_moves;
        extract_moves_with_explicit_src(bishop_moves & get_empty_fields(), legal_moves, bishop_idx);
        extract_captures_with_explicit_src(this, bishop_moves & get_enemy_fields(turn), legal_moves, bishop_idx);
    }
    return bishop_attacks;
}

uint64_t C_Board::collect_rook_moves_and_captures()
{
    uint64_t rook_attacks = empty_board;
    for (int rook_idx : scan_board(get_rooks(turn)))
    {
        uint64_t rook_moves = get_rook_moves_and_attacks(rook_idx, get_all_pieces());
        rook_attacks |= rook_moves;
        extract_moves_with_explicit_src(rook_moves & get_empty_fields(), legal_moves, rook_idx);
        extract_captures_with_explicit_src(this, rook_moves & get_enemy_fields(turn), legal_moves, rook_idx);
    }
    return rook_attacks;
}

uint64_t C_Board::collect_queen_moves_and_captures()
{
    uint64_t queen_attacks = empty_board;
    for (int queen_idx : scan_board(get_queen(turn)))
    {
        uint64_t queen_moves = get_bishop_moves_and_attacks(queen_idx, get_all_pieces()) | get_rook_moves_and_attacks(queen_idx, get_all_pieces());
        queen_attacks |= queen_moves;
        extract_moves_with_explicit_src(queen_moves & get_empty_fields(), legal_moves, queen_idx);
        extract_captures_with_explicit_src(this, queen_moves & get_enemy_fields(turn), legal_moves, queen_idx);
    }
    return queen_attacks;
}

inline uint64_t C_Board::collect_legal_moves()
{
    uint64_t all_attacks = empty_board;
    legal_moves = std::vector<move>();
    legal_moves.reserve(32);
    all_attacks |= collect_pawn_moves_and_captures();
    all_attacks |= collect_knight_moves_and_captures();
    all_attacks |= collect_king_moves_and_captures();
    all_attacks |= collect_bishop_moves_and_captures();
    all_attacks |= collect_rook_moves_and_captures();
    all_attacks |= collect_queen_moves_and_captures();
    return all_attacks;
}

inline void execute_move_forward(C_Board &board, move &m)
{
    uint8_t piece_type = get_piece_type_of_field(&board, m.src);
    board.unset_single_piece(board.turn, piece_type, m.src);
    if (m.capture)
    {
        board.unset_single_piece(board.turn * -1, m.capture, m.dest);
    }
    if (m.flag)
    {
        piece_type = m.flag;
    }
    board.set_single_piece(board.turn, piece_type, m.dest);
    if (m.castling)
    {
        if (m.castling == 1)
        {
            board.unset_single_piece(board.turn, pRook, 56);
            board.set_single_piece(board.turn, pRook, 40);
        }
        else if (m.castling == 2)
        {
            board.unset_single_piece(board.turn, pRook, 0);
            board.set_single_piece(board.turn, pRook, 24);
        }
    }
    if (m.ep)
    {
        board.unset_single_piece(board.turn, m.capture, m.dest - (int)board.turn);
    }
}

inline void execute_move_backward(C_Board &board, move &m)
{
    uint8_t piece_type = get_piece_type_of_field(&board, m.dest);
    board.unset_single_piece(board.turn * -1, piece_type, m.dest);
    if (m.capture)
    {
        board.set_single_piece(board.turn, m.capture, m.dest);
    }
    if (m.flag)
    {
        piece_type = pPawn;
    }
    board.set_single_piece(board.turn * -1, piece_type, m.src);
    if (m.castling)
    {
        if (m.castling == 1)
        {
            board.unset_single_piece(board.turn, pRook, 40);
            board.set_single_piece(board.turn, pRook, 56);
        }
        else if (m.castling == 2)
        {
            board.unset_single_piece(board.turn, pRook, 23);
            board.set_single_piece(board.turn, pRook, 0);
        }
    }
    if (m.ep)
    {
        board.set_single_piece(board.turn, m.capture, m.dest + (int)board.turn);
    }
}

inline void update_castling_rights(C_Board &board)
{
    uint64_t king_and_rooks = board.get_rooks(board.turn) | board.get_king(board.turn);
    std::array<uint64_t, 2> castling_masks = color_to_castling_mask.at(board.turn);
    std::array<castling, 2> castling_indicator = color_to_castling_indicator.at(board.turn);

    bool ks_castling_rights = (bool)(~(king_and_rooks ^ castling_masks[0]));
    bool qs_castling_rights = (bool)(~(king_and_rooks ^ castling_masks[1]));

    board.set_castling_rights(castling_indicator[0], ks_castling_rights);
    board.set_castling_rights(castling_indicator[1], qs_castling_rights);
}

inline bool check_castling_move_illegal(C_Board &board, move &m, uint64_t all_attacks)
{
    std::array<uint64_t, 2> no_attack_masks = color_to_castling_no_attack.at(board.turn * -1);
    bool match_kingside = (bool)(all_attacks & no_attack_masks[0]);
    bool match_queenside = (bool)(all_attacks & no_attack_masks[1]);
    return (m.castling == 1 && match_kingside) || (m.castling == 2 && match_queenside);
}

inline void C_Board::push_move(move m)
{
    execute_move_forward(*this, m);

    m.prev_c = castling_rights;
    update_castling_rights(*this);

    uint8_t ep_field = m.ep_field;
    m.ep_field = en_passant;
    en_passant = ep_field;

    // set counters
    uint8_t piece_type = get_piece_type_of_field(this, m.dest);
    m.prev_half_move_c = (uint8_t)half_moves;
    half_moves += 1.;
    half_moves *= ((!(piece_type == pPawn) && !(m.capture)));
    moves += (turn == -1.) * 1.;

    turn *= -1;

    uint64_t all_new_attacks = collect_legal_moves();
    king_attack = (bool)(all_new_attacks & get_king(-1. * turn));
    castling_move_illegal = check_castling_move_illegal(*this, m, all_new_attacks);

    move_stack.push_back(m);
}

inline move C_Board::pop_move()
{
    if (move_stack.empty())
    {
        return create_move(0, 0);
    }
    move m = move_stack.back();

    execute_move_backward(*this, m);

    castling_rights = m.prev_c;

    uint8_t ep_field = m.ep_field;
    m.ep_field = en_passant;
    en_passant = ep_field;

    // set_counters
    half_moves = m.prev_half_move_c;
    moves -= (turn == 1.) * 1.;

    turn *= -1;
    king_attack = false;
    castling_move_illegal = false;
    move_stack.pop_back();
    legal_moves = std::vector<move>();
    return m;
}