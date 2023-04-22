#pragma once

#include "base.h"

typedef struct {
    int moves[256];
    int count;
} moves;

static inline void add_move(moves* move_list, int move)
{
    move_list->moves[move_list->count] = move;
    move_list->count++;
}

static inline void generate_moves(moves* move_list)
{
    move_list->count = 0;

    int src, tgt;
    U64 piece_bb, attack_bb;

    for (int piece = P; piece <= k; piece++)
    {
        piece_bb = bitboard[piece];

        if (side)
        {
            if (piece == p)
            {
                while (piece_bb)
                {
                    src = LS1B(piece_bb);
                    tgt = src + 8;

                    if (!(tgt > h1) && !GET_BIT(occupancy[both], tgt))
                    {
                        if (src >= a2 && src <= h2)
                        {
                            add_move(move_list, ENCODE_MOVE(src, tgt, piece, q, 0, 0, 0, 0));
                            add_move(move_list, ENCODE_MOVE(src, tgt, piece, r, 0, 0, 0, 0));
                            add_move(move_list, ENCODE_MOVE(src, tgt, piece, b, 0, 0, 0, 0));
                            add_move(move_list, ENCODE_MOVE(src, tgt, piece, n, 0, 0, 0, 0));
                        }
                        else
                        {
                            add_move(move_list, ENCODE_MOVE(src, tgt, piece, 0, 0, 0, 0, 0));
                            if ((src >= a7 && src <= h7) && !GET_BIT(occupancy[both], (tgt + 8))) add_move(move_list, ENCODE_MOVE(src, (tgt + 8), piece, 0, 0, 1, 0, 0));
                        }
                    }
                    attack_bb = pawn_attacks[side][src] & occupancy[white];

                    while (attack_bb)
                    {
                        tgt = LS1B(attack_bb);

                        if (src >= a2 && src <= h2)
                        {
                            add_move(move_list, ENCODE_MOVE(src, tgt, piece, q, 1, 0, 0, 0));
                            add_move(move_list, ENCODE_MOVE(src, tgt, piece, r, 1, 0, 0, 0));
                            add_move(move_list, ENCODE_MOVE(src, tgt, piece, b, 1, 0, 0, 0));
                            add_move(move_list, ENCODE_MOVE(src, tgt, piece, n, 1, 0, 0, 0));
                        }

                        else add_move(move_list, ENCODE_MOVE(src, tgt, piece, 0, 1, 0, 0, 0));

                        POP_BIT(attack_bb, tgt);
                    }

                    if (enpassant != no_sq)
                    {
                        U64 enpassant_attacks = pawn_attacks[side][src] & (1ULL << enpassant);

                        if (enpassant_attacks)
                        {
                            int target_enpassant = LS1B(enpassant_attacks);
                            add_move(move_list, ENCODE_MOVE(src, target_enpassant, piece, 0, 1, 0, 1, 0));
                        }
                    }
                    POP_BIT(piece_bb, src);
                }
            }
            if (piece == k)
            {
                if (castle & bk) if (!GET_BIT(occupancy[both], f8) && !GET_BIT(occupancy[both], g8)) if (!is_square_attacked(e8, white) && !is_square_attacked(f8, white)) add_move(move_list, ENCODE_MOVE(e8, g8, piece, 0, 0, 0, 0, 1));
                if (castle & bq) if (!GET_BIT(occupancy[both], d8) && !GET_BIT(occupancy[both], c8) && !GET_BIT(occupancy[both], b8)) if (!is_square_attacked(e8, white) && !is_square_attacked(d8, white)) add_move(move_list, ENCODE_MOVE(e8, c8, piece, 0, 0, 0, 0, 1));
            }
        }
        else
        {
            if (piece == P)
            {
                while (piece_bb)
                {
                    src = LS1B(piece_bb);
                    tgt = src - 8;

                    if (!(tgt < a8) && !GET_BIT(occupancy[both], tgt))
                    {
                        if (src >= a7 && src <= h7)
                        {
                            add_move(move_list, ENCODE_MOVE(src, tgt, piece, Q, 0, 0, 0, 0));
                            add_move(move_list, ENCODE_MOVE(src, tgt, piece, R, 0, 0, 0, 0));
                            add_move(move_list, ENCODE_MOVE(src, tgt, piece, B, 0, 0, 0, 0));
                            add_move(move_list, ENCODE_MOVE(src, tgt, piece, N, 0, 0, 0, 0));
                        }

                        else
                        {
                            add_move(move_list, ENCODE_MOVE(src, tgt, piece, 0, 0, 0, 0, 0));
                            if ((src >= a2 && src <= h2) && !GET_BIT(occupancy[both], (tgt - 8))) add_move(move_list, ENCODE_MOVE(src, (tgt - 8), piece, 0, 0, 1, 0, 0));
                        }
                    }
                    attack_bb = pawn_attacks[side][src] & occupancy[black];

                    while (attack_bb)
                    {
                        tgt = LS1B(attack_bb);
                        if (src >= a7 && src <= h7)
                        {
                            add_move(move_list, ENCODE_MOVE(src, tgt, piece, Q, 1, 0, 0, 0));
                            add_move(move_list, ENCODE_MOVE(src, tgt, piece, R, 1, 0, 0, 0));
                            add_move(move_list, ENCODE_MOVE(src, tgt, piece, B, 1, 0, 0, 0));
                            add_move(move_list, ENCODE_MOVE(src, tgt, piece, N, 1, 0, 0, 0));
                        }
                        else add_move(move_list, ENCODE_MOVE(src, tgt, piece, 0, 1, 0, 0, 0));

                        POP_BIT(attack_bb, tgt);
                    }
                    if (enpassant != no_sq)
                    {
                        U64 enpassant_attacks = pawn_attacks[side][src] & (1ULL << enpassant);
                        if (enpassant_attacks)
                        {
                            int target_enpassant = LS1B(enpassant_attacks);
                            add_move(move_list, ENCODE_MOVE(src, target_enpassant, piece, 0, 1, 0, 1, 0));
                        }
                    }
                    POP_BIT(piece_bb, src);
                }
            }
            if (piece == K)
            {
                if (castle & wk) if (!GET_BIT(occupancy[both], f1) && !GET_BIT(occupancy[both], g1)) if (!is_square_attacked(e1, black) && !is_square_attacked(f1, black)) add_move(move_list, ENCODE_MOVE(e1, g1, piece, 0, 0, 0, 0, 1));
                if (castle & wq) if (!GET_BIT(occupancy[both], d1) && !GET_BIT(occupancy[both], c1) && !GET_BIT(occupancy[both], b1)) if (!is_square_attacked(e1, black) && !is_square_attacked(d1, black)) add_move(move_list, ENCODE_MOVE(e1, c1, piece, 0, 0, 0, 0, 1));
            }
        }

        if (side ? piece == n : piece == N)
        {
            while (piece_bb)
            {
                src = LS1B(piece_bb);
                attack_bb = knight_attacks[src] & ~occupancy[side];

                while (attack_bb)
                {
                    tgt = LS1B(attack_bb);

                    if (GET_BIT((side ? occupancy[white] : occupancy[black]), tgt)) add_move(move_list, ENCODE_MOVE(src, tgt, piece, 0, 1, 0, 0, 0));
                    else add_move(move_list, ENCODE_MOVE(src, tgt, piece, 0, 0, 0, 0, 0));

                    POP_BIT(attack_bb, tgt);
                }
                POP_BIT(piece_bb, src);
            }
        }

        if (side ? piece == b : piece == B)
        {
            while (piece_bb)
            {
                src = LS1B(piece_bb);
                attack_bb = get_bishop_attacks(src, occupancy[both]) & ~occupancy[side];

                while (attack_bb)
                {
                    tgt = LS1B(attack_bb);

                    if (GET_BIT((side ? occupancy[white] : occupancy[black]), tgt)) add_move(move_list, ENCODE_MOVE(src, tgt, piece, 0, 1, 0, 0, 0));
                    else add_move(move_list, ENCODE_MOVE(src, tgt, piece, 0, 0, 0, 0, 0));

                    POP_BIT(attack_bb, tgt);
                }
                POP_BIT(piece_bb, src);
            }
        }

        if (side ? piece == r : piece == R)
        {
            while (piece_bb)
            {
                src = LS1B(piece_bb);
                attack_bb = get_rook_attacks(src, occupancy[both]) & ~occupancy[side];

                while (attack_bb)
                {
                    tgt = LS1B(attack_bb);

                    if (GET_BIT((side ? occupancy[white] : occupancy[black]), tgt)) add_move(move_list, ENCODE_MOVE(src, tgt, piece, 0, 1, 0, 0, 0));
                    else add_move(move_list, ENCODE_MOVE(src, tgt, piece, 0, 0, 0, 0, 0));

                    POP_BIT(attack_bb, tgt);
                }
                POP_BIT(piece_bb, src);
            }
        }

        if (side ? piece == q : piece == Q)
        {
            while (piece_bb)
            {
                src = LS1B(piece_bb);
                attack_bb = get_queen_attacks(src, occupancy[both]) & ~occupancy[side];

                while (attack_bb)
                {
                    tgt = LS1B(attack_bb);

                    if (GET_BIT((side ? occupancy[white] : occupancy[black]), tgt)) add_move(move_list, ENCODE_MOVE(src, tgt, piece, 0, 1, 0, 0, 0));
                    else add_move(move_list, ENCODE_MOVE(src, tgt, piece, 0, 0, 0, 0, 0));

                    POP_BIT(attack_bb, tgt);
                }
                POP_BIT(piece_bb, src);
            }
        }

        if (side ? piece == k : piece == K)
        {
            while (piece_bb)
            {
                src = LS1B(piece_bb);
                attack_bb = king_attacks[src] & ~occupancy[side];

                while (attack_bb)
                {
                    tgt = LS1B(attack_bb);

                    if (GET_BIT((side ? occupancy[white] : occupancy[black]), tgt)) add_move(move_list, ENCODE_MOVE(src, tgt, piece, 0, 1, 0, 0, 0));
                    else add_move(move_list, ENCODE_MOVE(src, tgt, piece, 0, 0, 0, 0, 0));

                    POP_BIT(attack_bb, tgt);
                }
                POP_BIT(piece_bb, src);
            }
        }
    }
}

static inline int make_move(int move)
{
    COPY_BOARD();

    int source_square = GET_SOURCE(move);
    int target_square = GET_TARGET(move);
    int piece = GET_PIECE(move);
    int promoted_piece = GET_PROMOTED(move);
    int capture = GET_CAPTURE(move);
    int double_push = GET_DOUBLE(move);
    int enpass = GET_ENPASSANT(move);
    int castling = GET_CASTLING(move);

    POP_BIT(bitboard[piece], source_square);
    SET_BIT(bitboard[piece], target_square);

    if (capture)
    {
        int start_piece, end_piece;

        if (side == white)
        {
            start_piece = p;
            end_piece = k;
        }
        else
        {
            start_piece = P;
            end_piece = K;
        }

        for (int bb_piece = start_piece; bb_piece <= end_piece; bb_piece++)
        {
            if (GET_BIT(bitboard[bb_piece], target_square))
            {
                POP_BIT(bitboard[bb_piece], target_square);
                break;
            }
        }
    }

    if (promoted_piece)
    {
        POP_BIT(bitboard[side ? p : P], target_square);
        SET_BIT(bitboard[promoted_piece], target_square);
    }

    if (enpass)
    {
        side ? POP_BIT(bitboard[P], target_square - 8) : POP_BIT(bitboard[p], target_square + 8);
    }
    enpassant = no_sq;

    if (double_push)
    {
        side ? (enpassant = target_square - 8) : (enpassant = target_square + 8);
    }

    if (castling)
    {
        switch (target_square)
        {
        case (g1):
            POP_BIT(bitboard[R], h1);
            SET_BIT(bitboard[R], f1);
            break;
        case (c1):
            POP_BIT(bitboard[R], a1);
            SET_BIT(bitboard[R], d1);
            break;
        case (g8):
            POP_BIT(bitboard[r], h8);
            SET_BIT(bitboard[r], f8);
            break;
        case (c8):
            POP_BIT(bitboard[r], a8);
            SET_BIT(bitboard[r], d8);
            break;
        }
    }
    castle &= castling_rights[source_square];
    castle &= castling_rights[target_square];

    memset(occupancy, 0ULL, 24);

    for (int bb_piece = P; bb_piece <= K; bb_piece++) occupancy[white] |= bitboard[bb_piece];
    for (int bb_piece = p; bb_piece <= k; bb_piece++) occupancy[black] |= bitboard[bb_piece];

    occupancy[both] = occupancy[white] | occupancy[black];

    side ^= 1;

    if (is_square_attacked(side ? LS1B(bitboard[K]) : LS1B(bitboard[k]), side))
    {
        TAKE_BACK();
        return 0;
    }
    else return 1;
}

