#pragma once

#ifdef CHESSCONSOLE_EXPORTS
#define CHESSCONSOLE_API __declspec(dllexport)
#else
#define CHESSCONSOLE_API __declspec(dllimport)
#endif

extern "C" CHESSCONSOLE_API void initialise_attacks();
extern "C" CHESSCONSOLE_API void parse_fen(const char* fen);
extern "C" CHESSCONSOLE_API void print_board();
extern "C" CHESSCONSOLE_API long long perft_driver(int depth);
extern "C" CHESSCONSOLE_API int* get_valid_moves();
extern "C" CHESSCONSOLE_API void extern_make_move(int move);
extern "C" CHESSCONSOLE_API void take_bck();