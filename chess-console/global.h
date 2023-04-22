#pragma once

#include <utility>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>

typedef unsigned long long U64;

enum { a8, b8, c8, d8, e8, f8, g8, h8, a7, b7, c7, d7, e7, f7, g7, h7, a6, b6, c6, d6, e6, f6, g6, h6, a5, b5, c5, d5, e5, f5, g5, h5, a4, b4, c4, d4, e4, f4, g4, h4, a3, b3, c3, d3, e3, f3, g3, h3, a2, b2, c2, d2, e2, f2, g2, h2, a1, b1, c1, d1, e1, f1, g1, h1, no_sq };
enum { P, N, B, R, Q, K, p, n, b, r, q, k };
enum { white, black, both };
enum { rook, bishop };
enum { wk = 1, wq = 2, bk = 4, bq = 8 };

#define EMPTY_BOARD "8/8/8/8/8/8/8/8 b - - "
#define START_POSITION "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "

#define SET_BIT(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define GET_BIT(bitboard, square) ((bitboard) & (1ULL << (square)))
#define POP_BIT(bitboard, square) ((bitboard) &= ~(1ULL << (square)))
#define COUNT_BIT(bitboard) (int)__popcnt64(bitboard)
#define LS1B(bitboard) (int)_tzcnt_u64(bitboard)
//#define LS1B(bitboard) (int)__popcnt64((bitboard & (0 - bitboard)) - 1)

#define ENCODE_MOVE(src, tgt, piece, pro, cap, db, en, cas) (src) | (tgt << 6) | (piece << 12) | (pro << 16) | (cap << 20) | (db << 21) | (en << 22) | (cas << 23)

#define GET_SOURCE(move) (move & 0x3f)
#define GET_TARGET(move) ((move & 0xfc0) >> 6)
#define GET_PIECE(move) ((move & 0xf000) >> 12)
#define GET_PROMOTED(move) ((move & 0xf0000) >> 16)
#define GET_CAPTURE(move) (move & 0x100000)
#define GET_DOUBLE(move) (move & 0x200000)
#define GET_ENPASSANT(move) (move & 0x400000)
#define GET_CASTLING(move) (move & 0x800000)

#define COPY_BOARD()                                                    \
    U64 bitboard_copy[12], occupancy_copy[3];                           \
    int side_copy, enpassant_copy, castle_copy;                         \
    memcpy(bitboard_copy, bitboard, 96);                                \
    memcpy(occupancy_copy, occupancy, 24);                              \
    side_copy = side, enpassant_copy = enpassant, castle_copy = castle; \

#define TAKE_BACK()                                                     \
    memcpy(bitboard, bitboard_copy, 96);                                \
    memcpy(occupancy, occupancy_copy, 24);                              \
    side = side_copy, enpassant = enpassant_copy, castle = castle_copy; \


int side;
int enpassant = no_sq;
int castle;

const U64 not_a = 18374403900871474942ULL;
const U64 not_h = 9187201950435737471ULL;
const U64 not_hg = 4557430888798830399ULL;
const U64 not_ab = 18229723555195321596ULL;

const char* square2char[] = { "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8", "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7", "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6", "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4", "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3", "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2", "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1" };
const char* piece2char = "PNBRQKpnbrqk";
int char2piece[115];

const int castling_rights[64] = { 7, 15, 15, 15,  3, 15, 15, 11, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 13, 15, 15, 15, 12, 15, 15, 14 };

unsigned int random_state = 1804289383;

const int bishop_relevant_bits[64] = { 6, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 7, 7, 5, 5, 5, 5, 7, 9, 9, 7, 5, 5, 5, 5, 7, 9, 9, 7, 5, 5, 5, 5, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 6 };
const int rook_relevant_bits[64] = { 12, 11, 11, 11, 11, 11, 11, 12, 11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11, 12, 11, 11, 11, 11, 11, 11, 12 };
U64 rook_magic_numbers[64] = { 0x8a80104000800020ULL, 0x140002000100040ULL, 0x2801880a0017001ULL, 0x100081001000420ULL, 0x200020010080420ULL, 0x3001c0002010008ULL, 0x8480008002000100ULL, 0x2080088004402900ULL, 0x800098204000ULL, 0x2024401000200040ULL, 0x100802000801000ULL, 0x120800800801000ULL, 0x208808088000400ULL, 0x2802200800400ULL, 0x2200800100020080ULL, 0x801000060821100ULL, 0x80044006422000ULL, 0x100808020004000ULL, 0x12108a0010204200ULL, 0x140848010000802ULL, 0x481828014002800ULL, 0x8094004002004100ULL, 0x4010040010010802ULL, 0x20008806104ULL, 0x100400080208000ULL, 0x2040002120081000ULL, 0x21200680100081ULL, 0x20100080080080ULL, 0x2000a00200410ULL, 0x20080800400ULL, 0x80088400100102ULL, 0x80004600042881ULL, 0x4040008040800020ULL, 0x440003000200801ULL, 0x4200011004500ULL, 0x188020010100100ULL, 0x14800401802800ULL, 0x2080040080800200ULL, 0x124080204001001ULL, 0x200046502000484ULL, 0x480400080088020ULL, 0x1000422010034000ULL, 0x30200100110040ULL, 0x100021010009ULL, 0x2002080100110004ULL, 0x202008004008002ULL, 0x20020004010100ULL, 0x2048440040820001ULL, 0x101002200408200ULL, 0x40802000401080ULL, 0x4008142004410100ULL, 0x2060820c0120200ULL, 0x1001004080100ULL, 0x20c020080040080ULL, 0x2935610830022400ULL, 0x44440041009200ULL, 0x280001040802101ULL, 0x2100190040002085ULL, 0x80c0084100102001ULL, 0x4024081001000421ULL, 0x20030a0244872ULL, 0x12001008414402ULL, 0x2006104900a0804ULL, 0x1004081002402ULL };
U64 bishop_magic_numbers[64] = { 0x40040844404084ULL, 0x2004208a004208ULL, 0x10190041080202ULL, 0x108060845042010ULL, 0x581104180800210ULL, 0x2112080446200010ULL, 0x1080820820060210ULL, 0x3c0808410220200ULL, 0x4050404440404ULL, 0x21001420088ULL, 0x24d0080801082102ULL, 0x1020a0a020400ULL, 0x40308200402ULL, 0x4011002100800ULL, 0x401484104104005ULL, 0x801010402020200ULL, 0x400210c3880100ULL, 0x404022024108200ULL, 0x810018200204102ULL, 0x4002801a02003ULL, 0x85040820080400ULL, 0x810102c808880400ULL, 0xe900410884800ULL, 0x8002020480840102ULL, 0x220200865090201ULL, 0x2010100a02021202ULL, 0x152048408022401ULL, 0x20080002081110ULL, 0x4001001021004000ULL, 0x800040400a011002ULL, 0xe4004081011002ULL, 0x1c004001012080ULL, 0x8004200962a00220ULL, 0x8422100208500202ULL, 0x2000402200300c08ULL, 0x8646020080080080ULL, 0x80020a0200100808ULL, 0x2010004880111000ULL, 0x623000a080011400ULL, 0x42008c0340209202ULL, 0x209188240001000ULL, 0x400408a884001800ULL, 0x110400a6080400ULL, 0x1840060a44020800ULL, 0x90080104000041ULL, 0x201011000808101ULL, 0x1a2208080504f080ULL, 0x8012020600211212ULL, 0x500861011240000ULL, 0x180806108200800ULL, 0x4000020e01040044ULL, 0x300000261044000aULL, 0x802241102020002ULL, 0x20906061210001ULL, 0x5a84841004010310ULL, 0x4010801011c04ULL, 0xa010109502200ULL, 0x4a02012000ULL, 0x500201010098b028ULL, 0x8040002811040900ULL, 0x28000010020204ULL, 0x6000020202d0240ULL, 0x8918844842082200ULL, 0x4010011029020020ULL };

U64 bitboard[12];
U64 occupancy[3];

U64 bishop_masks[64];
U64 rook_masks[64];

U64 pawn_attacks[2][64];
U64 knight_attacks[64];
U64 king_attacks[64];
U64 bishop_attacks[64][512];
U64 rook_attacks[64][4096];
