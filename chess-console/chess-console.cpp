//References:
//Channel: Chess Programming -> Bit Board CHESS ENGINE in C
//Chess Programming Wiki

#include "pch.h"
#include "chess-console.h"

#include "base.h"

#pragma region input/output

void parse_fen(const char* fen)
{
    for (int i = 0; i < 12; ++i) char2piece[piece2char[i]] = i;
    memset(bitboard, 0ULL, sizeof(bitboard));
    memset(occupancy, 0ULL, sizeof(occupancy));

    side = 0;
    enpassant = no_sq;
    castle = 0;

    for (int rank = 0; rank < 8; rank++)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;

            if ((*fen >= 'a' && *fen <= 'z') || (*fen >= 'A' && *fen <= 'Z'))
            {
                int piece = char2piece[*fen];
                SET_BIT(bitboard[piece], square);
                fen++;
            }
            if (*fen >= '0' && *fen <= '9')
            {
                int offset = *fen - '0';
                int piece = -1;
                for (int bb_piece = P; bb_piece <= k; bb_piece++)
                {
                    if (GET_BIT(bitboard[bb_piece], square)) piece = bb_piece;
                }
                if (piece == -1) file--;
                file += offset;
                fen++;
            }
            if (*fen == '/') fen++;
        }
    }
    fen++;

    (*fen == 'w') ? (side = white) : (side = black);
    fen += 2;

    while (*fen != ' ')
    {
        switch (*fen)
        {
        case 'K': castle |= wk; break;
        case 'Q': castle |= wq; break;
        case 'k': castle |= bk; break;
        case 'q': castle |= bq; break;
        case '-': break;
        }
        fen++;
    }
    fen++;

    if (*fen != '-')
    {
        int file = fen[0] - 'a';
        int rank = 8 - (fen[1] - '0');
        enpassant = rank * 8 + file;
    }
    else enpassant = no_sq;

    for (int piece = P; piece <= K; piece++) occupancy[white] |= bitboard[piece];
    for (int piece = p; piece <= k; piece++) occupancy[black] |= bitboard[piece];

    occupancy[both] = occupancy[white] | occupancy[black];
}

void print_move(int move)
{
    printf("%s%s%c\n", square2char[GET_SOURCE(move)], square2char[GET_TARGET(move)], piece2char[GET_PROMOTED(move)]);
}

void print_move_list(moves* move_list)
{
    if (!move_list->count)
    {
        printf("\n     No move in the move list!\n");
        return;
    }
    printf("\n     move    piece     capture   double    enpass    castling\n\n");
    for (int move_count = 0; move_count < move_list->count; move_count++)
    {
        int move = move_list->moves[move_count];
        printf("      %s%s%c   %c         %d         %d         %d         %d\n", square2char[GET_SOURCE(move)],
            square2char[GET_TARGET(move)],
            GET_PROMOTED(move) ? piece2char[GET_PROMOTED(move)] : ' ',
            piece2char[GET_PIECE(move)],
            GET_CAPTURE(move) ? 1 : 0,
            GET_DOUBLE(move) ? 1 : 0,
            GET_ENPASSANT(move) ? 1 : 0,
            GET_CASTLING(move) ? 1 : 0);
    }
    printf("\n\n     Total number of moves: %d\n\n", move_list->count);
}

void print_bitboard(U64 bitboard)
{
    printf("\n");
    for (int rank = 0; rank < 8; rank++)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;

            if (!file) printf(" %d ", 8 - rank);
            printf(" %d", GET_BIT(bitboard, square) ? 1 : 0);
        }
        printf("\n");
    }
    printf("\n    a b c d e f g h\n\n");
    printf(" Decimal : %llud\n\n", bitboard);
}

void print_board()
{
    printf("\n");
    for (int rank = 0; rank < 8; rank++)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;

            if (!file) printf(" %d ", 8 - rank);

            int piece = -1;
            for (int bb_piece = P; bb_piece <= k; bb_piece++)
            {
                if (GET_BIT(bitboard[bb_piece], square)) piece = bb_piece;
            }
            printf(" %c", (piece == -1) ? '.' : piece2char[piece]);
        }
        printf("\n");
    }
    printf("\n    a b c d e f g h\n\n");
    printf(" side      : %s\n", !side ? "white" : "black");
    printf(" enpassant : %s\n", (enpassant != no_sq) ? square2char[enpassant] : "no");
    printf(" castling  : %c%c%c%c\n\n", (castle & wk) ? 'K' : '-', (castle & wq) ? 'Q' : '-', (castle & bk) ? 'k' : '-', (castle & bq) ? 'q' : '-');
}

void print_attacked_squares(int side)
{
    printf("\n");

    for (int rank = 0; rank < 8; rank++)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;

            if (!file) printf("  %d ", 8 - rank);
            printf(" %d", is_square_attacked(square, side) ? 1 : 0);
        }
        printf("\n");
    }
    printf("\n     a b c d e f g h\n\n");
}

#pragma endregion

#pragma region main_driver

void initialise_attacks()
{
    init_leaper_attacks();
    init_slider_attacks(bishop);
    init_slider_attacks(rook);
}

long long perft_driver(int depth)
{
    if (depth == 0)
    {
        return 1;
    }
    moves move_list[1];
    generate_moves(move_list);
    long long nodes = 0;
    for (int move_count = 0; move_count < move_list->count; move_count++)
    {
        COPY_BOARD();

        if (!make_move(move_list->moves[move_count])) continue;
        nodes = nodes + perft_driver(depth - 1);

        TAKE_BACK();
    }
    return nodes;
}

int arr[256];
int arr_size;

int* get_valid_moves()
{
    moves move_list[1];
    generate_moves(move_list);
    for (int i = 0; i < 256; i++)
    {
        arr[i] = 0;
    }
    for (int move_count = 0; move_count < move_list->count; move_count++)
    {
        COPY_BOARD();
        if (!make_move(move_list->moves[move_count])) continue;
        arr[move_count] = move_list->moves[move_count];
        TAKE_BACK();
    }
    return arr;
}

struct gameNode {
    U64 bitboard_copy[12], occupancy_copy[3];
    int side_copy, enpassant_copy, castle_copy;
    struct gameNode* next;
};
gameNode* top = NULL;

void push(U64 lbitboard[12], U64 loccupancy[3], int lside, int lenpassant, int lcastle) {
    struct gameNode* newNode;
    newNode = (struct gameNode*)malloc(sizeof(struct gameNode));

    memcpy(newNode->bitboard_copy, lbitboard, 96);
    memcpy(newNode->occupancy_copy, loccupancy, 24);
    newNode->side_copy = lside;
    newNode->enpassant_copy = lenpassant;
    newNode->castle_copy = lcastle;

    if (top == NULL) {
        newNode->next = NULL;
    }
    else {
        newNode->next = top;
    }
    top = newNode;
}

void pop() {
    if (top == NULL) {
        printf("\nStack Underflow\n");
    }
    else {
        struct gameNode* temp = top;

        memcpy(bitboard, top->bitboard_copy, 96);
        memcpy(occupancy, top->occupancy_copy, 24);
        side = top->side_copy;
        enpassant = top->enpassant_copy;
        castle = top->castle_copy;

        top = top->next;
        free(temp);
    }
}

void extern_make_move(int move)
{
    push(bitboard, occupancy, side, enpassant, castle);
    make_move(move);
}

void take_bck()
{
    pop();
}

#pragma endregion