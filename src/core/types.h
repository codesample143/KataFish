#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <string>
#include <string_view>

using namespace std;
using Eval = int;
using Bitboard = uint64_t;

/**
 *  Move one square up: x >> 8
    Move one square down: x << 8
    Move one square left: (x & not_file_a) >> 1
    Move one square right: (x & not_file_h) << 1

 */
enum GameStatus {
    IN_PROGRESS,
    CHECKMATE,
    DRAW_BY_REPETITION,
    DRAW_BY_STALEMATE,
    DRAW_BY_INSUFFICIENT_MATERIAL,
    DRAW_BY_50_MOVE_RULE
};

enum PieceType { 
    PAWN, //0
    KNIGHT, //1
    BISHOP, //2
    ROOK, //...
    QUEEN, 
    KING, 
    NO_PT 
};


enum Color { 
    WHITE, 
    BLACK 
};

enum Piece { 
    WP, //0
    WN, //1
    WB, //2
    WR, //....
    WQ, 
    WK, 
    BP, 
    BN, 
    BB, 
    BR, 
    BQ, 
    BK, 
    NO_PIECE 
};

enum File : int { 
    FILE_A, 
    FILE_B, 
    FILE_C, 
    FILE_D, 
    FILE_E, 
    FILE_F, 
    FILE_G, 
    FILE_H 
};


enum Rank : int { 
    RANK_8, 
    RANK_7, 
    RANK_6, 
    RANK_5, 
    RANK_4, 
    RANK_3, 
    RANK_2, 
    RANK_1 
};


constexpr array<Color, 2> COLORS = {
    WHITE, //0
    BLACK
};
constexpr array<PieceType, 6> PIECE_TYPES = {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING};
constexpr array<PieceType, 4> PROMOTION_PIECE_TYPES = {
    KNIGHT, 
    BISHOP, 
    ROOK, 
    QUEEN
};
constexpr array<string_view, 6> SAN_PTS = {"P", "N", "B", "R", "Q", "K"};

inline constexpr Piece ptToPiece(PieceType pt, Color side){
    return Piece(pt + 6 * side);
}

inline constexpr PieceType pieceToPT(Piece p){
    assert(p != NO_PIECE);
    return PieceType(p % 6);
}
/**
 * If p is WP (0) through WK (5), the expression p >= 6 is false (0).
   If p is BP (6) through BK (11), the expression p >= 6 is true (1).
 */

inline constexpr Color pieceColor(Piece p) {
    assert(p != NO_PIECE);
    return Color(p >= 6);
}

constexpr array<Piece, 6> WHITE_PIECES = {WP, WN, WB, WR, WQ, WK};

constexpr array<Piece, 6> BLACK_PIECES = {BP, BN, BB, BR, BQ, BK};

constexpr array<Piece, 12> ALL_PIECES = {WP, WN, WB, WR, WQ, WK, BP, BN, BB, BR, BQ, BK};

constexpr array<array<Piece, 6>, 2> COLOR_TO_PIECES = {WHITE_PIECES, BLACK_PIECES};

constexpr std::array<std::string_view, 12> PIECE_FILENAMES = {"wP", "wN", "wB", "wR", "wQ", "wK",
                                                              "bP", "bN", "bB", "bR", "bQ", "bK"};


//Forsyth-Edwards Notation (FEN) is a single-line ASCII string that represents a specific chess board position, including piece locations, whose turn it is, castling rights, and move counts.
/**
 * Maps FEN character to PieceType enum
 */
inline constexpr Piece fenCharToPiece(char c) {
    switch (c) {
        case 'P': return WP;
        case 'N': return WN;
        case 'B': return WB;
        case 'R': return WR;
        case 'Q': return WQ;
        case 'K': return WK;
        case 'p': return BP;
        case 'n': return BN;
        case 'b': return BB;
        case 'r': return BR;
        case 'q': return BQ;
        case 'k': return BK;
        default: return NO_PIECE;
    }
}
constexpr Eval infinity = 99999;
constexpr Eval MATE_SCORE = 99999;
const Eval MATE_BOUND = 99998;

constexpr int MAX_PLY = 128;

constexpr array<Eval, 7> pieceTypeValues = {100, 300, 320, 500, 900, infinity, 0};

enum Square : uint8_t {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1,
    NO_SQ
};

constexpr array<Square, NO_SQ> ALL_SQUARES = {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1,
};

inline constexpr Bitboard ALL_SQUARES_BB = 0xFFFFFFFFFFFFFFFF;

/**
 *  Move one square up: x >> 8
    Move one square down: x << 8
    Move one square left: (x & not_file_a) >> 1
    Move one square right: (x & not_file_h) << 1

 */


// (0, 0) is the top-left square (a8)
//The File (column) is determined by the lower 3 bits (values 0–7).
//The Rank (row) is determined by the upper 3 bits (values 8, 16, 24, 32, 40, 48, 56).
//When you XOR a square index with 56 (binary 111000), you are toggling the "Rank" bits while leaving the "File" bits untouched.
inline constexpr Square xyToSquare(int x, int y){
    return Square((y << 3) + x);
}

inline constexpr Square flipRank(Square s){
    return Square(s ^ a1);
}

// swap A8 <-> H8

inline constexpr Square flipFile(Square s) {
    return Square(s ^ h8);
}


constexpr Bitboard FILE_MASKS[8] = {
    72340172838076673ull,    // A
    144680345676153346ull,   // B
    289360691352306692ull,   // C
    578721382704613384ull,   // D
    1157442765409226768ull,  // E
    2314885530818453536ull,  // F
    4629771061636907072ull,  // G
    9259542123273814144ull   // H
};

constexpr Bitboard notA = ~FILE_MASKS[FILE_A];
constexpr Bitboard notAB = ~(FILE_MASKS[FILE_A] | FILE_MASKS[FILE_B]);
constexpr Bitboard notH = ~FILE_MASKS[FILE_H];
constexpr Bitboard notGH = ~(FILE_MASKS[FILE_G] | FILE_MASKS[FILE_H]);


constexpr Bitboard RANK_MASKS[8] = {
    255ull,                  // 8
    65280ull,                // 7
    16711680ull,             // 6
    4278190080ull,           // 5
    1095216660480ull,        // 4
    280375465082880ull,      // 3
    71776119061217280ull,    // 2
    18374686479671623680ull  // 1
};


constexpr Bitboard not1 = ~(RANK_MASKS[RANK_1]);
constexpr Bitboard not12 = ~(RANK_MASKS[RANK_1] | RANK_MASKS[RANK_2]);
constexpr Bitboard not8 = ~(RANK_MASKS[RANK_8]);
constexpr Bitboard not78 = ~(RANK_MASKS[RANK_7] | RANK_MASKS[RANK_8]);


inline constexpr File fileOf(Square sq) {
    return File(sq & 7);  // same as sq % 8
}

inline constexpr Rank rankOf(Square sq) {
    return Rank(sq >> 3);  // same as sq / 8
}


