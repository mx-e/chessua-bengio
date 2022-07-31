#ifndef TRANSPOSIITON
#define TRANSPOSIITON
#include "utils.hpp"
#include "constants.hpp"
#include <unordered_map>

struct TT_Entry
{
    int depth;
    move best_move;
};

typedef std::unordered_map<u_int64_t, TT_Entry> TT_HashTable;

typedef u_int64_t Feature;

const int FEATURES = 64 * 12 + 1;
typedef std::array<Feature, FEATURES> Features;

typedef std::mersenne_twister_engine<unsigned long long, 64, 312, 156,
                                     31, 0xb5026f5aa96619e9ULL,
                                     29, 0x5555555555555555ULL,
                                     17, 0x71d67fffeda60000ULL,
                                     37, 0xfff7eee000000000ULL,
                                     43, 6364136223846793005ULL>
    mt19937_64;

struct HashState
{
    u_int64_t hash;
    Features features;
    TT_HashTable tt_ht;
};

inline void construct_hash(HashState &hash_state)
{
    mt19937_64 generator(32953951);
    for (int i = 0; i < FEATURES; i++)
    {
        hash_state.features[i] = generator();
    }
}

struct HashCache
{
    uint8_t piece_type;
    float color;
};

inline int
get_piece_index(float color, uint8_t piece_type)
{
    return piece_type + (color == White ? 5 : -1);
}

inline void xor_at_piece_index(HashState &hash_state, int at, int piece_idx)
{
    hash_state.hash ^= hash_state.features[1 + at * 12 + piece_idx];
}

inline void xor_at_index(HashState &hash_state, C_BoardState &board_state, int at, HashCache &cache)
{
    cache.piece_type = get_piece_type_of_field(board_state, at);
    cache.color = get_color_of_field(board_state, at);

    if (cache.piece_type > 0)
    {
        int piece_idx = get_piece_index(cache.color, cache.piece_type);
        xor_at_piece_index(hash_state, at, piece_idx);
    }
}

inline void compute_hash(HashState &hash_state, C_BoardState &board_state)
{
    hash_state.hash = board_state.turn == Black ? hash_state.features[0] : 0;
    for (int i = 0; i < 64; i++)
    {
        HashCache cache;
        xor_at_index(hash_state, board_state, i, cache);
    }
}

inline void update_hash(HashState &hash_state, C_BoardState &board_state, move &move)
{
    HashCache cache;
    xor_at_index(hash_state, board_state, move.src, cache);

    int piece_type = move.promotion ? move.promotion : cache.piece_type;
    int piece_idx = get_piece_index(cache.color, piece_type);

    xor_at_piece_index(hash_state, move.dest, piece_idx);

    if (move.capture)
        xor_at_index(hash_state, board_state, move.dest, cache);

    if (move.ep)
        xor_at_index(hash_state, board_state, move.dest - (int)board_state.turn, cache);
}

inline void reverse_hash(HashState &hash_state, C_BoardState &board_state, move &move)
{
    HashCache cache;
    if (move.ep)
        xor_at_index(hash_state, board_state, move.dest - (int)board_state.turn, cache);

    if (move.capture)
        xor_at_index(hash_state, board_state, move.dest, cache);

    cache.piece_type = get_piece_type_of_field(board_state, move.src);
    cache.color = get_color_of_field(board_state, move.src);

    int piece_type = move.promotion ? move.promotion : cache.piece_type;
    int piece_idx = get_piece_index(cache.color, piece_type);

    xor_at_piece_index(hash_state, move.dest, piece_idx);
    xor_at_index(hash_state, board_state, move.src, cache);
}

inline std::optional<TT_Entry> find_tt_entry(HashState &hash_state)
{
    TT_HashTable::const_iterator potential_entry = hash_state.tt_ht.find(hash_state.hash);
    if (potential_entry != hash_state.tt_ht.end())
        return hash_state.tt_ht[hash_state.hash];
    return std::optional<TT_Entry>();
}

inline void hash_move(HashState &hash_state, move m, int depth)
{
    if (MV_HASH_ENABLED) {
        move mm = m;
        hash_state.tt_ht[hash_state.hash] = TT_Entry{depth, mm};
    }
        
}

#endif
