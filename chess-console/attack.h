#pragma once

#include "base.h"

U64 set_occupancy(int index, int bit_count, U64 attack_mask)
{
    U64 occupancy_bb = 0ULL;

    for (int count = 0; count < bit_count; count++)
    {
        int sq = LS1B(attack_mask);
        POP_BIT(attack_mask, sq);

        if (index & (1 << count)) occupancy_bb |= (1ULL << sq);
    }
    return occupancy_bb;
}

U64 mask_pawn_attacks(int side, int square)
{
    U64 attack_bb = 0ULL, piece_bb = 0ULL;

    SET_BIT(piece_bb, square);

    if (!side)
    {
        if ((piece_bb >> 7) & not_a) attack_bb |= (piece_bb >> 7);
        if ((piece_bb >> 9) & not_h) attack_bb |= (piece_bb >> 9);
    }
    else
    {
        if ((piece_bb << 7) & not_h) attack_bb |= (piece_bb << 7);
        if ((piece_bb << 9) & not_a) attack_bb |= (piece_bb << 9);
    }
    return attack_bb;
}

U64 mask_knight_attacks(int square)
{
    U64 attack_bb = 0ULL, piece_bb = 0ULL;

    SET_BIT(piece_bb, square);

    if ((piece_bb >> 17) & not_h) attack_bb |= (piece_bb >> 17);
    if ((piece_bb >> 15) & not_a) attack_bb |= (piece_bb >> 15);
    if ((piece_bb >> 10) & not_hg) attack_bb |= (piece_bb >> 10);
    if ((piece_bb >> 6) & not_ab) attack_bb |= (piece_bb >> 6);
    if ((piece_bb << 17) & not_a) attack_bb |= (piece_bb << 17);
    if ((piece_bb << 15) & not_h) attack_bb |= (piece_bb << 15);
    if ((piece_bb << 10) & not_ab) attack_bb |= (piece_bb << 10);
    if ((piece_bb << 6) & not_hg) attack_bb |= (piece_bb << 6);

    return attack_bb;
}

U64 mask_king_attacks(int square)
{
    U64 attack_bb = 0ULL, piece_bb = 0ULL;

    SET_BIT(piece_bb, square);

    if (piece_bb >> 8) attack_bb |= (piece_bb >> 8);
    if ((piece_bb >> 9) & not_h) attack_bb |= (piece_bb >> 9);
    if ((piece_bb >> 7) & not_a) attack_bb |= (piece_bb >> 7);
    if ((piece_bb >> 1) & not_h) attack_bb |= (piece_bb >> 1);
    if (piece_bb << 8) attack_bb |= (piece_bb << 8);
    if ((piece_bb << 9) & not_a) attack_bb |= (piece_bb << 9);
    if ((piece_bb << 7) & not_h) attack_bb |= (piece_bb << 7);
    if ((piece_bb << 1) & not_a) attack_bb |= (piece_bb << 1);

    return attack_bb;
}

U64 mask_bishop_attacks(int square)
{
    U64 attack_bb = 0ULL;
    int r, f;
    int tr = square / 8;
    int tf = square % 8;

    for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++) attack_bb |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++) attack_bb |= (1ULL << (r * 8 + f));
    for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--) attack_bb |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--) attack_bb |= (1ULL << (r * 8 + f));

    return attack_bb;
}

U64 mask_rook_attacks(int square)
{
    U64 attack_bb = 0ULL;
    int r, f;
    int tr = square / 8;
    int tf = square % 8;

    for (r = tr + 1; r <= 6; r++) attack_bb |= (1ULL << (r * 8 + tf));
    for (r = tr - 1; r >= 1; r--) attack_bb |= (1ULL << (r * 8 + tf));
    for (f = tf + 1; f <= 6; f++) attack_bb |= (1ULL << (tr * 8 + f));
    for (f = tf - 1; f >= 1; f--) attack_bb |= (1ULL << (tr * 8 + f));

    return attack_bb;
}

U64 bishop_attacks_on_the_fly(int square, U64 block)
{
    U64 attack_bb = 0ULL;
    int r, f;
    int tr = square / 8;
    int tf = square % 8;

    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++)
    {
        attack_bb |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }
    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++)
    {
        attack_bb |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }
    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--)
    {
        attack_bb |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }
    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--)
    {
        attack_bb |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }

    return attack_bb;
}

U64 rook_attacks_on_the_fly(int square, U64 block)
{
    U64 attack_bb = 0ULL;
    int r, f;
    int tr = square / 8;
    int tf = square % 8;

    for (r = tr + 1; r <= 7; r++)
    {
        attack_bb |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & block) break;
    }
    for (r = tr - 1; r >= 0; r--)
    {
        attack_bb |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & block) break;
    }
    for (f = tf + 1; f <= 7; f++)
    {
        attack_bb |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & block) break;
    }
    for (f = tf - 1; f >= 0; f--)
    {
        attack_bb |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & block) break;
    }

    return attack_bb;
}

void init_leaper_attacks()
{
    for (int square = 0; square < 64; square++)
    {
        pawn_attacks[white][square] = mask_pawn_attacks(white, square);
        pawn_attacks[black][square] = mask_pawn_attacks(black, square);

        knight_attacks[square] = mask_knight_attacks(square);
        king_attacks[square] = mask_king_attacks(square);
    }
}

void init_slider_attacks(int flag)
{
    for (int square = 0; square < 64; square++)
    {
        bishop_masks[square] = mask_bishop_attacks(square);
        rook_masks[square] = mask_rook_attacks(square);

        U64 attack_mask = flag ? bishop_masks[square] : rook_masks[square];

        int relevant_bits_count = COUNT_BIT(attack_mask);
        int occupancy_indicies = (1 << relevant_bits_count);

        for (int index = 0; index < occupancy_indicies; index++)
        {
            if (flag)
            {
                U64 occupancy = set_occupancy(index, relevant_bits_count, attack_mask);
                int magic_index = (int)((occupancy * bishop_magic_numbers[square]) >> (64 - bishop_relevant_bits[square]));
                bishop_attacks[square][magic_index] = bishop_attacks_on_the_fly(square, occupancy);
            }
            else
            {
                U64 occupancy = set_occupancy(index, relevant_bits_count, attack_mask);
                int magic_index = (int)((occupancy * rook_magic_numbers[square]) >> (64 - rook_relevant_bits[square]));
                rook_attacks[square][magic_index] = rook_attacks_on_the_fly(square, occupancy);
            }
        }
    }
}

static inline U64 get_bishop_attacks(int square, U64 occupancy)
{
    occupancy &= bishop_masks[square];
    occupancy *= bishop_magic_numbers[square];
    occupancy >>= 64 - bishop_relevant_bits[square];

    return bishop_attacks[square][occupancy];
}

static inline U64 get_rook_attacks(int square, U64 occupancy)
{
    occupancy &= rook_masks[square];
    occupancy *= rook_magic_numbers[square];
    occupancy >>= 64 - rook_relevant_bits[square];

    return rook_attacks[square][occupancy];
}

static inline U64 get_queen_attacks(int square, U64 occupancy)
{
    U64 bishop_occupancy = occupancy, rook_occupancy = occupancy;

    bishop_occupancy &= bishop_masks[square];
    bishop_occupancy *= bishop_magic_numbers[square];
    bishop_occupancy >>= 64 - bishop_relevant_bits[square];

    rook_occupancy &= rook_masks[square];
    rook_occupancy *= rook_magic_numbers[square];
    rook_occupancy >>= 64 - rook_relevant_bits[square];

    return rook_attacks[square][rook_occupancy] | bishop_attacks[square][bishop_occupancy];
}

static inline int is_square_attacked(int square, int side)
{
    if (side == white && (pawn_attacks[black][square] & bitboard[P])) return 1;
    if (side == black && (pawn_attacks[white][square] & bitboard[p])) return 1;

    if (knight_attacks[square] & (side ? bitboard[n] : bitboard[N])) return 1;
    if (get_bishop_attacks(square, occupancy[both]) & (side ? bitboard[b] : bitboard[B])) return 1;
    if (get_rook_attacks(square, occupancy[both]) & (side ? bitboard[r] : bitboard[R])) return 1;
    if (get_queen_attacks(square, occupancy[both]) & (side ? bitboard[q] : bitboard[Q])) return 1;
    if (king_attacks[square] & (side ? bitboard[k] : bitboard[K])) return 1;

    return 0;
}
