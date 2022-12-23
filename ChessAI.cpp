#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include <stdlib.h>
#include <time.h>
#include <unordered_map>

typedef long long BitBoard;

void outputBinary(long long x)
{
    std::string output = "";

    for (int i = 63; i >= 0; i--)
    {
        if (x & (1ll << i))
            output += '1';
        else
            output += '0';
    }
    output += '\n';

    std::cout << output;
}

void outputBitBoard(BitBoard x)
{
    std::string output = "";

    for (int i = 0; i < 64; i++)
    {
        if (x & (1ll << i))
            output += '1';
        else
            output += '0';

        if ((i + 1) % 8 == 0)
            output += '\n';
    }

    std::cout << output;
}

inline char toChar(int x)
{
    return x + '0';
}

struct Status
{
public:

    BitBoard whitePawn;
    BitBoard whiteRook;
    BitBoard whiteKnight;
    BitBoard whiteBishop;
    BitBoard whiteQueen;
    BitBoard whiteKing;

    BitBoard blackPawn;
    BitBoard blackRook;
    BitBoard blackKnight;
    BitBoard blackBishop;
    BitBoard blackQueen;
    BitBoard blackKing;

    BitBoard whitePiece;
    BitBoard blackPiece;

    BitBoard occupied;
    BitBoard empty;

    BitBoard enPassant;

    BitBoard whitePawnAttackZones;
    BitBoard whiteRookAttackZones;
    BitBoard whiteKnightAttackZones;
    BitBoard whiteBishopAttackZones;
    BitBoard whiteQueenAttackZones;
    BitBoard whiteKingAttackZones;

    BitBoard blackPawnAttackZones;
    BitBoard blackRookAttackZones;
    BitBoard blackKnightAttackZones;
    BitBoard blackBishopAttackZones;
    BitBoard blackQueenAttackZones;
    BitBoard blackKingAttackZones;

    BitBoard whitePieceAttackZones;
    BitBoard blackPieceAttackZones;

    bool whiteKingMoved;
    bool whiteLeftRookMoved;
    bool whiteRightRookMoved;

    bool blackKingMoved;
    bool blackLeftRookMoved;
    bool blackRightRookMoved;

    bool whiteKingCastled;
    bool blackKingCastled;

    std::vector<std::string> whiteMoves;
    std::vector<std::string> blackMoves;
};

class Chess
{
public:

    BitBoard whitePawn;
    BitBoard whiteRook;
    BitBoard whiteKnight;
    BitBoard whiteBishop;
    BitBoard whiteQueen;
    BitBoard whiteKing;

    BitBoard blackPawn;
    BitBoard blackRook;
    BitBoard blackKnight;
    BitBoard blackBishop;
    BitBoard blackQueen;
    BitBoard blackKing;

    BitBoard whitePiece;
    BitBoard blackPiece;

    BitBoard occupied;
    BitBoard empty;

    BitBoard enPassant;

    BitBoard rank[8];
    BitBoard file[8];
    BitBoard diagonal1[15];
    BitBoard diagonal2[15];

    BitBoard whiteLeftSide;
    BitBoard whiteRightSide;
    BitBoard whiteExtendedRightSide;

    BitBoard blackLeftSide;
    BitBoard blackRightSide;
    BitBoard blackExtendedRightSide;

    int whiteLeftRook;
    int whiteRightRook;

    int blackLeftRook;
    int blackRightRook;

    BitBoard whitePawnAttackZones;
    BitBoard whiteRookAttackZones;
    BitBoard whiteKnightAttackZones;
    BitBoard whiteBishopAttackZones;
    BitBoard whiteQueenAttackZones;
    BitBoard whiteKingAttackZones;

    BitBoard blackPawnAttackZones;
    BitBoard blackRookAttackZones;
    BitBoard blackKnightAttackZones;
    BitBoard blackBishopAttackZones;
    BitBoard blackQueenAttackZones;
    BitBoard blackKingAttackZones;

    BitBoard whitePieceAttackZones;
    BitBoard blackPieceAttackZones;

    std::vector<std::string> movesHistory;

    std::vector<Status> prevStatuses;

    static const int ATTACK_MODULO = 6701; /// Cred ca am putea un modul si mai mic pentru functia de hash, ca sa economisim memorie.
    static const int ATTACK_SIZE = 2 * ATTACK_MODULO;

    BitBoard knightAttackZone[64];
    BitBoard rankAttackZone[64][ATTACK_SIZE];
    BitBoard fileAttackZone[64][ATTACK_SIZE];
    BitBoard diagonal1AttackZone[64][ATTACK_SIZE];
    BitBoard diagonal2AttackZone[64][ATTACK_SIZE];
    BitBoard kingAttackZone[64];

    /// Diagonala 1 este cea principala, iar numaratoarea incepe din coltul sus dreapta, iar coltul sus stanga este originea.
    /// Diagonala 2 este cea secundara dintr-o matrice, iar numaratoarea ei incepe din coltul jos dreapta. Originea tot sus in stanga.

    char board[64] =
    {
        'R', 'N', 'B', 'K', 'Q', 'B', 'N', 'R',
        'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',
        'r', 'n', 'b', 'k', 'q', 'b', 'n', 'r',
    };

    char nothing[7][18] =
    {
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    };

    char whitePawnPiece[7][18] =
    {
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' '},
    };

    char whiteRookPiece[7][18] =
    {
        {' ', ' ', ' ', ' ', 'N', 'N', ' ', 'N', ' ', ' ', 'N', ' ', 'N', 'N', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' '},
    };

    char whiteKnightPiece[7][18] =
    {
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', 'N', ' ', 'N', 'N', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', 'N', 'N', ' ', ' ', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' '},
    };

    char whiteBishopPiece[7][18] =
    {
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'N', 'N', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'N', 'N', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', 'N', 'N', 'N', ' ', 'N', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', 'N', 'N', ' ', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' '},
    };

    char whiteQueenPiece[7][18] =
    {
        {' ', ' ', ' ', ' ', 'N', ' ', ' ', 'N', ' ', ' ', 'N', ' ', ' ', 'N', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', 'N', 'N', ' ', 'N', ' ', ' ', 'N', ' ', 'N', 'N', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' '},
    };

    char whiteKingPiece[7][18] =
    {
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', 'N', 'N', ' ', 'N', 'N', ' ', 'N', 'N', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', 'N', ' ', ' ', 'N', 'N', 'N', 'N', ' ', ' ', 'N', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', 'N', 'N', ' ', ' ', 'N', 'N', ' ', ' ', 'N', 'N', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', ' ', ' ', ' ', ' ', ' '},
    };

    char blackPawnPiece[7][18] =
    {
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', '!', '!', '!', '!', '!', '!', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', '!', '!', '!', '!', '!', '!', '!', '!', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', '!', '!', '!', '!', '!', '!', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', '!', '!', '!', '!', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', '!', '!', '!', '!', '!', '!', '!', '!', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', '!', '!', '!', '!', '!', '!', '!', '!', '!', '!', ' ', ' ', ' ', ' '},
    };

    char blackRookPiece[7][18] =
    {
        {' ', ' ', ' ', ' ', '!', '!', ' ', '!', ' ', ' ', '!', ' ', '!', '!', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', '!', '!', '!', '!', '!', '!', '!', '!', '!', '!', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', '!', '!', '!', '!', '!', '!', '!', '!', '!', '!', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', '!', '!', '!', '!', '!', '!', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', '!', '!', '!', '!', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', '!', '!', '!', '!', '!', '!', '!', '!', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', '!', '!', '!', '!', '!', '!', '!', '!', '!', '!', ' ', ' ', ' ', ' '},
    };

    char blackKnightPiece[7][18] =
    {
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '!', '!', '!', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', '!', '!', '!', '!', '!', '!', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', '!', ' ', '!', '!', '!', '!', '!', '!', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', '!', '!', '!', '!', '!', '!', '!', '!', '!', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', '!', '!', ' ', ' ', '!', '!', '!', '!', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', '!', '!', '!', '!', '!', '!', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', '!', '!', '!', '!', '!', '!', '!', '!', ' ', ' ', ' ', ' '},
    };

    char blackBishopPiece[7][18] =
    {
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '!', '!', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', '!', '!', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', '!', '!', '!', ' ', '!', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', '!', '!', ' ', '!', '!', '!', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', '!', '!', '!', '!', '!', '!', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', '!', '!', '!', '!', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', '!', '!', '!', '!', '!', '!', '!', '!', ' ', ' ', ' ', ' ', ' '},
    };

    char blackQueenPiece[7][18] =
    {
        {' ', ' ', ' ', ' ', '!', ' ', ' ', '!', ' ', ' ', '!', ' ', ' ', '!', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', '!', '!', ' ', '!', ' ', ' ', '!', ' ', '!', '!', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', '!', '!', '!', '!', '!', '!', '!', '!', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', '!', '!', '!', '!', '!', '!', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', '!', '!', '!', '!', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', '!', '!', '!', '!', '!', '!', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', '!', '!', '!', '!', '!', '!', '!', '!', '!', '!', ' ', ' ', ' ', ' '},
    };

    char blackKingPiece[7][18] =
    {
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', '!', '!', '!', '!', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', '!', '!', ' ', '!', '!', ' ', '!', '!', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', '!', ' ', ' ', '!', '!', '!', '!', ' ', ' ', '!', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', '!', '!', ' ', ' ', '!', '!', ' ', ' ', '!', '!', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', '!', '!', '!', '!', '!', '!', '!', '!', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', '!', '!', '!', '!', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', '!', '!', '!', '!', '!', '!', '!', '!', ' ', ' ', ' ', ' ', ' '},
    };

    char displayBoard[81][169];

    bool whiteKingMoved;
    bool whiteLeftRookMoved;
    bool whiteRightRookMoved;

    bool blackKingMoved;
    bool blackLeftRookMoved;
    bool blackRightRookMoved;

    bool whiteKingCastled;
    bool blackKingCastled;

    std::vector<std::string> whiteMoves;
    std::vector<std::string> blackMoves;

    std::unordered_map<std::string, std::pair<int, std::string>> positionScore;
    std::unordered_map<std::string, int> positionFreq;

    static const int LOG2_MODULO = 67;
    static const int LOG2_SIZE = 2 * LOG2_MODULO;
    int log2[LOG2_SIZE];

    const int DIAGONAL_DIM[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 7, 6, 5, 4, 3, 2, 1 };

public:

    Chess()
    {
        this->whitePawn = 0ll;
        this->whiteRook = 0ll;
        this->whiteKnight = 0ll;
        this->whiteBishop = 0ll;
        this->whiteQueen = 0ll;
        this->whiteKing = 0ll;

        this->blackPawn = 0ll;
        this->blackRook = 0ll;
        this->blackKnight = 0ll;
        this->blackBishop = 0ll;
        this->blackQueen = 0ll;
        this->blackKing = 0ll;

        this->whitePiece = 0ll;
        this->blackPiece = 0ll;

        this->occupied = 0ll;
        this->empty = 0ll;

        this->enPassant = 0ll;

        for (int i = 0; i < 8; i++)
            this->rank[i] = 0ll;

        for (int i = 0; i < 8; i++)
            this->file[i] = 0ll;

        this->whitePawnAttackZones = 0ll;
        this->whiteRookAttackZones = 0ll;
        this->whiteKnightAttackZones = 0ll;
        this->whiteBishopAttackZones = 0ll;
        this->whiteQueenAttackZones = 0ll;
        this->whiteKingAttackZones = 0ll;

        this->blackPawnAttackZones = 0ll;
        this->blackRookAttackZones = 0ll;
        this->blackKnightAttackZones = 0ll;
        this->blackBishopAttackZones = 0ll;
        this->blackQueenAttackZones = 0ll;
        this->blackKingAttackZones = 0ll;

        this->whitePieceAttackZones = 0ll;
        this->blackPieceAttackZones = 0ll;

        this->whiteKingMoved = false;
        this->whiteLeftRookMoved = false;
        this->whiteRightRookMoved = false;

        this->blackKingMoved = false;
        this->blackLeftRookMoved = false;
        this->blackRightRookMoved = false;

        this->whiteKingCastled = false;
        this->blackKingCastled = false;

        this->whiteLeftSide = 0ll;
        this->whiteRightSide = 0ll;
        this->whiteExtendedRightSide = 0ll;

        this->blackLeftSide = 0ll;
        this->blackRightSide = 0ll;
        this->blackExtendedRightSide = 0ll;

        this->whiteLeftSide = this->whiteLeftSide | (1ll << 1) | (1ll << 2);
        this->whiteRightSide = this->whiteRightSide | (1ll << 4) | (1ll << 5);
        this->whiteExtendedRightSide = this->whiteRightSide | (1ll << 6);

        this->blackLeftSide = this->blackLeftSide | (1ll << 57) | (1ll << 58);
        this->blackRightSide = this->blackRightSide | (1ll << 60) | (1ll << 61);
        this->blackExtendedRightSide = this->blackRightSide | (1ll << 62);

        for (int i = 0; i < 64; i += 8)
            for (int j = i; j < i + 8; j++)
                this->rank[i / 8] |= (1ll << j);

        for (int i = 0; i < 8; i++)
            for (int j = i; j < 64; j += 8)
                this->file[i] |= (1ll << j);

        for (int i = 0, x = 0, y = 7; i < 15; i++)
        {
            this->diagonal1[i] = 0ll;

            int crtX = x;
            int crtY = y;

            while (crtX < 8 && crtY < 8)
            {
                this->diagonal1[i] |= (1ll << (crtX * 8 + crtY));
                crtX++;
                crtY++;
            }

            if (y == 0)
                x++;
            else
                y--;
        }

        for (int i = 0, x = 7, y = 7; i < 15; i++)
        {
            this->diagonal2[i] = 0ll;

            int crtX = x;
            int crtY = y;

            while (crtX < 8 && crtY >= 0)
            {
                this->diagonal2[i] |= (1ll << (crtX * 8 + crtY));
                crtX++;
                crtY--;
            }

            if (x == 0)
                y--;
            else
                x--;
        }

        for (int i = 0; i < LOG2_SIZE; i++)
            this->log2[i] = 0;

        for (int i = 0; i < 64; i++)
            this->log2[(1ll << i) % LOG2_MODULO + LOG2_MODULO] = i;

        for (int i = 0; i < 64; i++)
        {
            this->knightAttackZone[i] = 0ll;

            if (i >= 6)
                this->knightAttackZone[i] |= ((1ll << (i - 6)) & ~this->file[0] & ~this->file[1]);
            if (i >= 10)
                this->knightAttackZone[i] |= ((1ll << (i - 10)) & ~this->file[6] & ~this->file[7]);
            if (i >= 15)
                this->knightAttackZone[i] |= ((1ll << (i - 15)) & ~this->file[0]);
            if (i >= 17)
                this->knightAttackZone[i] |= ((1ll << (i - 17)) & ~this->file[7]);
            if (i + 6 < 64)
                this->knightAttackZone[i] |= ((1ll << (i + 6)) & ~this->file[6] & ~this->file[7]);
            if (i + 10 < 64)
                this->knightAttackZone[i] |= ((1ll << (i + 10)) & ~this->file[0] & ~this->file[1]);
            if (i + 15 < 64)
                this->knightAttackZone[i] |= ((1ll << (i + 15)) & ~this->file[7] & ~this->rank[0]);
            if (i + 17 < 64)
                this->knightAttackZone[i] |= ((1ll << (i + 17)) & ~this->file[0] & ~this->rank[0]);
        }

        for (int i = 0; i < 64; i++)
            for (int j = 0; j < ATTACK_SIZE; j++)
            {
                this->rankAttackZone[i][j] = 0ll;
                this->fileAttackZone[i][j] = 0ll;
                this->diagonal1AttackZone[i][j] = 0ll;
                this->diagonal2AttackZone[i][j] = 0ll;
            }

        for (int i = 0; i < 64; i++)
        {
            for (int j = 0; j < 256; j++)
            {
                if ((1ll << i) & ((1ll * j) << (i / 8 * 8)))
                    continue;

                BitBoard rank = (1ll * j) << (i / 8 * 8);
                BitBoard sol = 0ll;

                int crtX = i / 8;

                int crtY = i % 8 - 1;
                while (crtY >= 0 && !((1ll << (crtX * 8 + crtY)) & rank))
                {
                    sol |= (1ll << (crtX * 8 + crtY));
                    crtY--;
                }
                if (crtY >= 0)
                    sol |= (1ll << (crtX * 8 + crtY));

                crtY = i % 8 + 1;
                while (crtY < 8 && !((1ll << (crtX * 8 + crtY)) & rank))
                {
                    sol |= (1ll << (crtX * 8 + crtY));
                    crtY++;
                }
                if (crtY < 8)
                    sol |= (1ll << (crtX * 8 + crtY));

                this->rankAttackZone[i][rank % ATTACK_MODULO + ATTACK_MODULO] = sol;
            }
        }

        for (int i = 0; i < 64; i++)
        {
            for (int j = 0; j < 256; j++)
            {
                BitBoard file = 0ll;

                BitBoard bits = j;
                BitBoard bit = bits & ~(bits - 1);

                while (bit)
                {
                    int bitPos = this->log2[bit % LOG2_MODULO + LOG2_MODULO];

                    file |= (1ll << (bitPos * 8 + i % 8));

                    bits &= ~bit;
                    bit = bits & ~(bits - 1);
                }

                if ((1ll << i) & file)
                    continue;

                BitBoard sol = 0ll;

                int crtY = i % 8;

                int crtX = i / 8 - 1;
                while (crtX >= 0 && !(1ll << (crtX * 8 + crtY) & file))
                {
                    sol |= (1ll << (crtX * 8 + crtY));
                    crtX--;
                }
                if (crtX >= 0)
                    sol |= (1ll << (crtX * 8 + crtY));

                crtX = i / 8 + 1;
                while (crtX < 8 && !(1ll << (crtX * 8 + crtY) & file))
                {
                    sol |= (1ll << (crtX * 8 + crtY));
                    crtX++;
                }
                if (crtX < 8)
                    sol |= (1ll << (crtX * 8 + crtY));

                this->fileAttackZone[i][file % ATTACK_MODULO + ATTACK_MODULO] = sol;
            }
        }

        for (int i = 0; i < 64; i++)
        {
            for (int j = 0; j < 256; j++)
            {
                if (j >= (1 << this->DIAGONAL_DIM[7 - i % 8 + i / 8]))
                    continue;

                BitBoard diagonal = 0ll;

                BitBoard bits = j;
                BitBoard bit = bits & ~(bits - 1);

                int minDist = std::min(i / 8, i % 8);

                int edgeX = i / 8 - minDist;
                int edgeY = i % 8 - minDist;

                while (bit)
                {
                    int bitPos = this->log2[bit % LOG2_MODULO + LOG2_MODULO];

                    diagonal |= (1ll << ((edgeX + bitPos) * 8 + edgeY + bitPos));

                    bits &= ~bit;
                    bit = bits & ~(bits - 1);
                }

                if ((1ll << i) & diagonal)
                    continue;

                BitBoard sol = 0ll;

                int crtX = i / 8 - 1;
                int crtY = i % 8 - 1;

                while (crtX >= 0 && crtY >= 0 && !((1ll << (crtX * 8 + crtY)) & diagonal))
                {
                    sol |= (1ll << (crtX * 8 + crtY));
                    crtX--;
                    crtY--;
                }
                if (crtX >= 0 && crtY >= 0)
                    sol |= (1ll << (crtX * 8 + crtY));

                crtX = i / 8 + 1;
                crtY = i % 8 + 1;

                while (crtX < 8 && crtY < 8 && !((1ll << (crtX * 8 + crtY)) & diagonal))
                {
                    sol |= (1ll << (crtX * 8 + crtY));
                    crtX++;
                    crtY++;
                }
                if (crtX < 8 && crtY < 8)
                    sol |= (1ll << (crtX * 8 + crtY));

                this->diagonal1AttackZone[i][diagonal % ATTACK_MODULO + ATTACK_MODULO] = sol;
            }
        }

        for (int i = 0; i < 64; i++)
        {
            for (int j = 0; j < 256; j++)
            {
                if (j >= (1 << this->DIAGONAL_DIM[7 - i / 8 + 7 - i % 8]))
                    continue;

                BitBoard diagonal = 0ll;

                BitBoard bits = j;
                BitBoard bit = bits & ~(bits - 1);

                int minDist = std::min(i / 8, 7 - i % 8);

                int edgeX = i / 8 - minDist;
                int edgeY = i % 8 + minDist;

                while (bit)
                {
                    int bitPos = this->log2[bit % LOG2_MODULO + LOG2_MODULO];

                    diagonal |= (1ll << ((edgeX + bitPos) * 8 + edgeY - bitPos));

                    bits &= ~bit;
                    bit = bits & ~(bits - 1);
                }

                if ((1ll << i) & diagonal)
                    continue;

                BitBoard sol = 0ll;

                int crtX = i / 8 - 1;
                int crtY = i % 8 + 1;

                while (crtX >= 0 && crtY < 8 && !((1ll << (crtX * 8 + crtY)) & diagonal))
                {
                    sol |= (1ll << (crtX * 8 + crtY));
                    crtX--;
                    crtY++;
                }
                if (crtX >= 0 && crtY < 8)
                    sol |= (1ll << (crtX * 8 + crtY));

                crtX = i / 8 + 1;
                crtY = i % 8 - 1;

                while (crtX < 8 && crtY >= 0 && !((1ll << (crtX * 8 + crtY)) & diagonal))
                {
                    sol |= (1ll << (crtX * 8 + crtY));
                    crtX++;
                    crtY--;
                }
                if (crtX < 8 && crtY >= 0)
                    sol |= (1ll << (crtX * 8 + crtY));

                this->diagonal2AttackZone[i][diagonal % ATTACK_MODULO + ATTACK_MODULO] = sol;
            }
        }

        for (int i = 0; i < 64; i++)
        {
            this->kingAttackZone[i] = 0ll;

            if (i >= 1)
                this->kingAttackZone[i] |= ((1ll << (i - 1)) & ~this->file[7]);
            if (i >= 7)
                this->kingAttackZone[i] |= ((1ll << (i - 7)) & ~this->file[0]);
            if (i >= 8)
                this->kingAttackZone[i] |= ((1ll << (i - 8)));
            if (i >= 9)
                this->kingAttackZone[i] |= ((1ll << (i - 9)) & ~this->file[7]);
            if (i + 1 < 64)
                this->kingAttackZone[i] |= ((1ll << (i + 1)) & ~this->file[0]);
            if (i + 7 < 64)
                this->kingAttackZone[i] |= ((1ll << (i + 7)) & ~this->file[7]);
            if (i + 8 < 64)
                this->kingAttackZone[i] |= ((1ll << (i + 8)));
            if (i + 9 < 64)
                this->kingAttackZone[i] |= ((1ll << (i + 9)) & ~this->file[0]);
        }

        for (int i = 0; i < 81; i++)
            for (int j = 0; j < 169; j++)
            {
                if (i % 10 == 0 || j % 21 == 0)
                    displayBoard[i][j] = 'X';
                else
                    displayBoard[i][j] = ' ';
            }

        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++)
            {
                displayBoard[(i - 1) * 10 + 1][(j - 1) * 21 + 1] = i + '0';
                displayBoard[(i - 1) * 10 + 1][(j - 1) * 21 + 2] = 'a' + 8 - j;
            }

        this->movesHistory.clear();
    }

    void initializeBoard()
    {
        for (int i = 0; i < 64; i++)
        {
            if (this->board[i] == 'P')
                this->whitePawn |= (1ll << i);
            else if (this->board[i] == 'R')
                this->whiteRook |= (1ll << i);
            else if (this->board[i] == 'N')
                this->whiteKnight |= (1ll << i);
            else if (this->board[i] == 'B')
                this->whiteBishop |= (1ll << i);
            else if (this->board[i] == 'Q')
                this->whiteQueen |= (1ll << i);
            else if (this->board[i] == 'K')
                this->whiteKing |= (1ll << i);

            else if (this->board[i] == 'p')
                this->blackPawn |= (1ll << i);
            else if (this->board[i] == 'r')
                this->blackRook |= (1ll << i);
            else if (this->board[i] == 'n')
                this->blackKnight |= (1ll << i);
            else if (this->board[i] == 'b')
                this->blackBishop |= (1ll << i);
            else if (this->board[i] == 'q')
                this->blackQueen |= (1ll << i);
            else if (this->board[i] == 'k')
                this->blackKing |= (1ll << i);
        }

        this->whitePiece = this->whitePawn | this->whiteRook | this->whiteKnight | this->whiteBishop | this->whiteQueen | this->whiteKing;
        this->blackPiece = this->blackPawn | this->blackRook | this->blackKnight | this->blackBishop | this->blackQueen | this->blackKing;

        this->occupied = this->whitePiece | this->blackPiece;

        this->empty = ~this->occupied;

        this->whiteLeftRook = 0;
        this->whiteRightRook = 7;

        this->blackLeftRook = 56;
        this->blackRightRook = 63;
    }

    inline void putNothing(int x, int y)
    {
        for (int i = x * 10 + 2; i <= (x + 1) * 10 - 2; i++)
            for (int j = y * 21 + 2; j <= (y + 1) * 21 - 2; j++)
                this->displayBoard[i][j] = this->nothing[i - x * 10 - 2][j - y * 21 - 2];
    }

    inline void putWhitePawnPiece(int x, int y)
    {
        for (int i = x * 10 + 2; i <= (x + 1) * 10 - 2; i++)
            for (int j = y * 21 + 2; j <= (y + 1) * 21 - 2; j++)
                this->displayBoard[i][j] = this->whitePawnPiece[i - x * 10 - 2][j - y * 21 - 2];
    }

    inline void putWhiteRookPiece(int x, int y)
    {
        for (int i = x * 10 + 2; i <= (x + 1) * 10 - 2; i++)
            for (int j = y * 21 + 2; j <= (y + 1) * 21 - 2; j++)
                this->displayBoard[i][j] = this->whiteRookPiece[i - x * 10 - 2][j - y * 21 - 2];
    }

    inline void putWhiteKnightPiece(int x, int y)
    {
        for (int i = x * 10 + 2; i <= (x + 1) * 10 - 2; i++)
            for (int j = y * 21 + 2; j <= (y + 1) * 21 - 2; j++)
                this->displayBoard[i][j] = this->whiteKnightPiece[i - x * 10 - 2][j - y * 21 - 2];
    }

    inline void putWhiteBishopPiece(int x, int y)
    {
        for (int i = x * 10 + 2; i <= (x + 1) * 10 - 2; i++)
            for (int j = y * 21 + 2; j <= (y + 1) * 21 - 2; j++)
                this->displayBoard[i][j] = this->whiteBishopPiece[i - x * 10 - 2][j - y * 21 - 2];
    }

    inline void putWhiteQueenPiece(int x, int y)
    {
        for (int i = x * 10 + 2; i <= (x + 1) * 10 - 2; i++)
            for (int j = y * 21 + 2; j <= (y + 1) * 21 - 2; j++)
                this->displayBoard[i][j] = this->whiteQueenPiece[i - x * 10 - 2][j - y * 21 - 2];
    }

    inline void putWhiteKingPiece(int x, int y)
    {
        for (int i = x * 10 + 2; i <= (x + 1) * 10 - 2; i++)
            for (int j = y * 21 + 2; j <= (y + 1) * 21 - 2; j++)
                this->displayBoard[i][j] = this->whiteKingPiece[i - x * 10 - 2][j - y * 21 - 2];
    }

    inline void putBlackPawnPiece(int x, int y)
    {
        for (int i = x * 10 + 2; i <= (x + 1) * 10 - 2; i++)
            for (int j = y * 21 + 2; j <= (y + 1) * 21 - 2; j++)
                this->displayBoard[i][j] = this->blackPawnPiece[i - x * 10 - 2][j - y * 21 - 2];
    }

    inline void putBlackRookPiece(int x, int y)
    {
        for (int i = x * 10 + 2; i <= (x + 1) * 10 - 2; i++)
            for (int j = y * 21 + 2; j <= (y + 1) * 21 - 2; j++)
                this->displayBoard[i][j] = this->blackRookPiece[i - x * 10 - 2][j - y * 21 - 2];
    }

    inline void putBlackKnightPiece(int x, int y)
    {
        for (int i = x * 10 + 2; i <= (x + 1) * 10 - 2; i++)
            for (int j = y * 21 + 2; j <= (y + 1) * 21 - 2; j++)
                this->displayBoard[i][j] = this->blackKnightPiece[i - x * 10 - 2][j - y * 21 - 2];
    }

    inline void putBlackBishopPiece(int x, int y)
    {
        for (int i = x * 10 + 2; i <= (x + 1) * 10 - 2; i++)
            for (int j = y * 21 + 2; j <= (y + 1) * 21 - 2; j++)
                this->displayBoard[i][j] = this->blackBishopPiece[i - x * 10 - 2][j - y * 21 - 2];
    }

    inline void putBlackQueenPiece(int x, int y)
    {
        for (int i = x * 10 + 2; i <= (x + 1) * 10 - 2; i++)
            for (int j = y * 21 + 2; j <= (y + 1) * 21 - 2; j++)
                this->displayBoard[i][j] = this->blackQueenPiece[i - x * 10 - 2][j - y * 21 - 2];
    }

    inline void putBlackKingPiece(int x, int y)
    {
        for (int i = x * 10 + 2; i <= (x + 1) * 10 - 2; i++)
            for (int j = y * 21 + 2; j <= (y + 1) * 21 - 2; j++)
                this->displayBoard[i][j] = this->blackKingPiece[i - x * 10 - 2][j - y * 21 - 2];
    }

    void refreshBoard()
    {
        for (int i = 0; i < 64; i++)
        {
            if (this->whitePawn & (1ll << i))
                this->board[i] = 'P';
            else if (this->whiteRook & (1ll << i))
                this->board[i] = 'R';
            else if (this->whiteKnight & (1ll << i))
                this->board[i] = 'N';
            else if (this->whiteBishop & (1ll << i))
                this->board[i] = 'B';
            else if (this->whiteQueen & (1ll << i))
                this->board[i] = 'Q';
            else if (this->whiteKing & (1ll << i))
                this->board[i] = 'K';

            else if (this->blackPawn & (1ll << i))
                this->board[i] = 'p';
            else if (this->blackRook & (1ll << i))
                this->board[i] = 'r';
            else if (this->blackKnight & (1ll << i))
                this->board[i] = 'n';
            else if (this->blackBishop & (1ll << i))
                this->board[i] = 'b';
            else if (this->blackQueen & (1ll << i))
                this->board[i] = 'q';
            else if (this->blackKing & (1ll << i))
                this->board[i] = 'k';
            else
                this->board[i] = ' ';
        }

        for (int i = 0; i < 64; i++)
        {
            if (this->board[i] == ' ')
                putNothing(i / 8, i % 8);
            else if (this->board[i] == 'P')
                putWhitePawnPiece(i / 8, i % 8);
            else if (this->board[i] == 'R')
                putWhiteRookPiece(i / 8, i % 8);
            else if (this->board[i] == 'N')
                putWhiteKnightPiece(i / 8, i % 8);
            else if (this->board[i] == 'B')
                putWhiteBishopPiece(i / 8, i % 8);
            else if (this->board[i] == 'Q')
                putWhiteQueenPiece(i / 8, i % 8);
            else if (this->board[i] == 'K')
                putWhiteKingPiece(i / 8, i % 8);
            else if (this->board[i] == 'p')
                putBlackPawnPiece(i / 8, i % 8);
            else if (this->board[i] == 'r')
                putBlackRookPiece(i / 8, i % 8);
            else if (this->board[i] == 'n')
                putBlackKnightPiece(i / 8, i % 8);
            else if (this->board[i] == 'b')
                putBlackBishopPiece(i / 8, i % 8);
            else if (this->board[i] == 'q')
                putBlackQueenPiece(i / 8, i % 8);
            else
                putBlackKingPiece(i / 8, i % 8);
        }
    }

    void outputBoard(int displayMode)
    {
        std::string output = "";

        if (displayMode == 1)
        {
            char auxDisplayBoard[81][169];

            for (int i = 0; i < 81; i += 10)
            {
                for (int j = 0; j < 169; j++)
                    auxDisplayBoard[i][j] = this->displayBoard[i][j];

                if (i <= 70)
                {
                    int iAux = 70 - i;

                    for (int j = 0; j < 169; j++)
                    {
                        for (int k = 0; k <= 9; k++)
                            auxDisplayBoard[iAux + k][j] = this->displayBoard[i + k][j];
                    }
                }
            }

            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 81; i++)
                {
                    for (int k = 0; k < 21; k++)
                    {
                        std::swap(auxDisplayBoard[i][j * 21 + k], auxDisplayBoard[i][(7 - j) * 21 + k]);
                    }
                }
            }

            for (int i = 0; i < 81; i++)
            {
                for (int j = 0; j < 169; j++)
                    output += auxDisplayBoard[i][j];
                output += '\n';
            }
        }
        else
        {
            for (int i = 0; i < 81; i++)
            {
                for (int j = 0; j < 169; j++)
                    output += this->displayBoard[i][j];
                output += '\n';
            }
        }

        std::cout << output;
    }

    void createPrevStatus()
    {
        this->prevStatuses.emplace_back();

        this->prevStatuses.back().whitePawn = this->whitePawn;
        this->prevStatuses.back().whiteRook = this->whiteRook;
        this->prevStatuses.back().whiteKnight = this->whiteKnight;
        this->prevStatuses.back().whiteBishop = this->whiteBishop;
        this->prevStatuses.back().whiteQueen = this->whiteQueen;
        this->prevStatuses.back().whiteKing = this->whiteKing;

        this->prevStatuses.back().blackPawn = this->blackPawn;
        this->prevStatuses.back().blackRook = this->blackRook;
        this->prevStatuses.back().blackKnight = this->blackKnight;
        this->prevStatuses.back().blackBishop = this->blackBishop;
        this->prevStatuses.back().blackQueen = this->blackQueen;
        this->prevStatuses.back().blackKing = this->blackKing;

        this->prevStatuses.back().whitePiece = this->whitePiece;
        this->prevStatuses.back().blackPiece = this->blackPiece;

        this->prevStatuses.back().occupied = this->occupied;
        this->prevStatuses.back().empty = this->empty;

        this->prevStatuses.back().enPassant = this->enPassant;

        this->prevStatuses.back().whitePawnAttackZones = this->whitePawnAttackZones;
        this->prevStatuses.back().whiteRookAttackZones = this->whiteRookAttackZones;
        this->prevStatuses.back().whiteKnightAttackZones = this->whiteKnightAttackZones;
        this->prevStatuses.back().whiteBishopAttackZones = this->whiteBishopAttackZones;
        this->prevStatuses.back().whiteQueenAttackZones = this->whiteQueenAttackZones;
        this->prevStatuses.back().whiteKingAttackZones = this->whiteKingAttackZones;

        this->prevStatuses.back().blackPawnAttackZones = this->blackPawnAttackZones;
        this->prevStatuses.back().blackRookAttackZones = this->blackRookAttackZones;
        this->prevStatuses.back().blackKnightAttackZones = this->blackKnightAttackZones;
        this->prevStatuses.back().blackBishopAttackZones = this->blackBishopAttackZones;
        this->prevStatuses.back().blackQueenAttackZones = this->blackQueenAttackZones;
        this->prevStatuses.back().blackKingAttackZones = this->blackKingAttackZones;

        this->prevStatuses.back().whitePieceAttackZones = this->whitePieceAttackZones;
        this->prevStatuses.back().blackPieceAttackZones = this->blackPieceAttackZones;

        this->prevStatuses.back().whiteKingMoved = this->whiteKingMoved;
        this->prevStatuses.back().whiteLeftRookMoved = this->whiteLeftRookMoved;
        this->prevStatuses.back().whiteRightRookMoved = this->whiteRightRookMoved;

        this->prevStatuses.back().blackKingMoved = this->blackKingMoved;
        this->prevStatuses.back().blackLeftRookMoved = this->blackLeftRookMoved;
        this->prevStatuses.back().blackRightRookMoved = this->blackRightRookMoved;

        this->prevStatuses.back().whiteMoves = this->whiteMoves;
        this->prevStatuses.back().blackMoves = this->blackMoves;

        this->prevStatuses.back().whiteKingCastled = this->whiteKingCastled;
        this->prevStatuses.back().blackKingCastled = this->blackKingCastled;
    }

    void loadPrevStatus()
    {
        this->whitePawn = this->prevStatuses.back().whitePawn;
        this->whiteRook = this->prevStatuses.back().whiteRook;
        this->whiteKnight = this->prevStatuses.back().whiteKnight;
        this->whiteBishop = this->prevStatuses.back().whiteBishop;
        this->whiteQueen = this->prevStatuses.back().whiteQueen;
        this->whiteKing = this->prevStatuses.back().whiteKing;

        this->blackPawn = this->prevStatuses.back().blackPawn;
        this->blackRook = this->prevStatuses.back().blackRook;
        this->blackKnight = this->prevStatuses.back().blackKnight;
        this->blackBishop = this->prevStatuses.back().blackBishop;
        this->blackQueen = this->prevStatuses.back().blackQueen;
        this->blackKing = this->prevStatuses.back().blackKing;

        this->whitePiece = this->prevStatuses.back().whitePiece;
        this->blackPiece = this->prevStatuses.back().blackPiece;

        this->occupied = this->prevStatuses.back().occupied;
        this->empty = this->prevStatuses.back().empty;

        this->enPassant = this->prevStatuses.back().enPassant;

        this->whitePawnAttackZones = this->prevStatuses.back().whitePawnAttackZones;
        this->whiteRookAttackZones = this->prevStatuses.back().whiteRookAttackZones;
        this->whiteKnightAttackZones = this->prevStatuses.back().whiteKnightAttackZones;
        this->whiteBishopAttackZones = this->prevStatuses.back().whiteBishopAttackZones;
        this->whiteQueenAttackZones = this->prevStatuses.back().whiteQueenAttackZones;
        this->whiteKingAttackZones = this->prevStatuses.back().whiteKingAttackZones;

        this->blackPawnAttackZones = this->prevStatuses.back().blackPawnAttackZones;
        this->blackRookAttackZones = this->prevStatuses.back().blackRookAttackZones;
        this->blackKnightAttackZones = this->prevStatuses.back().blackKnightAttackZones;
        this->blackBishopAttackZones = this->prevStatuses.back().blackBishopAttackZones;
        this->blackQueenAttackZones = this->prevStatuses.back().blackQueenAttackZones;
        this->blackKingAttackZones = this->prevStatuses.back().blackKingAttackZones;

        this->whitePieceAttackZones = this->prevStatuses.back().whitePieceAttackZones;
        this->blackPieceAttackZones = this->prevStatuses.back().blackPieceAttackZones;

        this->whiteKingMoved = this->prevStatuses.back().whiteKingMoved;
        this->whiteLeftRookMoved = this->prevStatuses.back().whiteLeftRookMoved;
        this->whiteRightRookMoved = this->prevStatuses.back().whiteRightRookMoved;

        this->blackKingMoved = this->prevStatuses.back().blackKingMoved;
        this->blackLeftRookMoved = this->prevStatuses.back().blackLeftRookMoved;
        this->blackRightRookMoved = this->prevStatuses.back().blackRightRookMoved;

        this->whiteMoves = this->prevStatuses.back().whiteMoves;
        this->blackMoves = this->prevStatuses.back().blackMoves;

        this->whiteKingCastled = this->prevStatuses.back().whiteKingCastled;
        this->blackKingCastled = this->prevStatuses.back().blackKingCastled;
    }

    void generateWhitePawnMoves()
    {
        this->whitePawnAttackZones = 0ll;

        /// Toate reperele sunt fata de matricea din afisare, cum o vedem noi.

        /// Atac stanga

        BitBoard pieces = (this->whitePawn << 7) & this->blackPiece & ~this->rank[7] & ~this->file[7];
        BitBoard piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            this->whitePawnAttackZones |= (1ll << pos);
            this->whiteMoves.emplace_back();
            this->whiteMoves.back() = this->whiteMoves.back() + 'P' + toChar(pos / 8 - 1) + toChar(pos % 8 + 1) + toChar(pos / 8) + toChar(pos % 8) + ' ';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        /// Atac dreapta

        pieces = (this->whitePawn << 9) & this->blackPiece & ~this->rank[7] & ~this->file[0];
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            this->whitePawnAttackZones |= (1ll << pos);
            this->whiteMoves.emplace_back();
            this->whiteMoves.back() = this->whiteMoves.back() + 'P' + toChar(pos / 8 - 1) + toChar(pos % 8 - 1) + toChar(pos / 8) + toChar(pos % 8) + ' ';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        /// Mers inainte o casuta

        pieces = (this->whitePawn << 8) & this->empty & ~this->rank[7];
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            this->whiteMoves.emplace_back();
            this->whiteMoves.back() = this->whiteMoves.back() + 'P' + toChar(pos / 8 - 1) + toChar(pos % 8) + toChar(pos / 8) + toChar(pos % 8) + ' ';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        /// Mers inainte doua casute

        pieces = ((this->whitePawn & this->rank[1]) << 16) & (this->empty << 8) & this->empty;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            this->whiteMoves.emplace_back();
            this->whiteMoves.back() = this->whiteMoves.back() + 'P' + toChar(pos / 8 - 2) + toChar(pos % 8) + toChar(pos / 8) + toChar(pos % 8) + ' ';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        /// Promovari

        /// Promovare prin atac stanga

        pieces = (this->whitePawn << 7) & this->blackPiece & this->rank[7] & ~this->file[7];
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            this->whitePawnAttackZones |= (1ll << pos);
            this->whiteMoves.emplace_back();
            this->whiteMoves.back() = this->whiteMoves.back() + 'P' + toChar(pos / 8 - 1) + toChar(pos % 8 + 1) + toChar(pos / 8) + toChar(pos % 8) + 'R';
            this->whiteMoves.emplace_back();
            this->whiteMoves.back() = this->whiteMoves.back() + 'P' + toChar(pos / 8 - 1) + toChar(pos % 8 + 1) + toChar(pos / 8) + toChar(pos % 8) + 'N';
            this->whiteMoves.emplace_back();
            this->whiteMoves.back() = this->whiteMoves.back() + 'P' + toChar(pos / 8 - 1) + toChar(pos % 8 + 1) + toChar(pos / 8) + toChar(pos % 8) + 'B';
            this->whiteMoves.emplace_back();
            this->whiteMoves.back() = this->whiteMoves.back() + 'P' + toChar(pos / 8 - 1) + toChar(pos % 8 + 1) + toChar(pos / 8) + toChar(pos % 8) + 'Q';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        /// Promovare prin atac dreapta

        pieces = (this->whitePawn << 9) & this->blackPiece & this->rank[7] & ~this->file[0];
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            this->whitePawnAttackZones |= (1ll << pos);
            this->whiteMoves.emplace_back();
            this->whiteMoves.back() = this->whiteMoves.back() + 'P' + toChar(pos / 8 - 1) + toChar(pos % 8 - 1) + toChar(pos / 8) + toChar(pos % 8) + 'R';
            this->whiteMoves.emplace_back();
            this->whiteMoves.back() = this->whiteMoves.back() + 'P' + toChar(pos / 8 - 1) + toChar(pos % 8 - 1) + toChar(pos / 8) + toChar(pos % 8) + 'N';
            this->whiteMoves.emplace_back();
            this->whiteMoves.back() = this->whiteMoves.back() + 'P' + toChar(pos / 8 - 1) + toChar(pos % 8 - 1) + toChar(pos / 8) + toChar(pos % 8) + 'B';
            this->whiteMoves.emplace_back();
            this->whiteMoves.back() = this->whiteMoves.back() + 'P' + toChar(pos / 8 - 1) + toChar(pos % 8 - 1) + toChar(pos / 8) + toChar(pos % 8) + 'Q';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        /// Promovare prin mers inainte o casuta

        pieces = (this->whitePawn << 8) & this->empty & this->rank[7];
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            this->whiteMoves.emplace_back();
            this->whiteMoves.back() = this->whiteMoves.back() + 'P' + toChar(pos / 8 - 1) + toChar(pos % 8) + toChar(pos / 8) + toChar(pos % 8) + 'R';
            this->whiteMoves.emplace_back();
            this->whiteMoves.back() = this->whiteMoves.back() + 'P' + toChar(pos / 8 - 1) + toChar(pos % 8) + toChar(pos / 8) + toChar(pos % 8) + 'N';
            this->whiteMoves.emplace_back();
            this->whiteMoves.back() = this->whiteMoves.back() + 'P' + toChar(pos / 8 - 1) + toChar(pos % 8) + toChar(pos / 8) + toChar(pos % 8) + 'B';
            this->whiteMoves.emplace_back();
            this->whiteMoves.back() = this->whiteMoves.back() + 'P' + toChar(pos / 8 - 1) + toChar(pos % 8) + toChar(pos / 8) + toChar(pos % 8) + 'Q';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        /// En passant

        /// Atac stanga

        pieces = this->whitePawn & (this->enPassant << 1) & this->rank[4];
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            this->whitePawnAttackZones |= (1ll << (pos - 1));
            this->whiteMoves.emplace_back();
            this->whiteMoves.back() = this->whiteMoves.back() + 'P' + toChar(pos / 8) + toChar(pos % 8) + toChar(pos / 8 + 1) + toChar(pos % 8 - 1) + '+';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        /// Atac dreapta

        pieces = (this->whitePawn << 1) & this->enPassant & this->rank[4];
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            this->whitePawnAttackZones |= (1ll << pos);
            this->whiteMoves.emplace_back();
            this->whiteMoves.back() = this->whiteMoves.back() + 'P' + toChar(pos / 8) + toChar(pos % 8 - 1) + toChar(pos / 8 + 1) + toChar(pos % 8) + '+';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }
    }

    void generateWhiteRookMoves()
    {
        this->whiteRookAttackZones = 0ll;

        BitBoard pieces;
        BitBoard piece;

        pieces = this->whiteRook;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int piecePos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            BitBoard rank = this->occupied & this->rank[piecePos / 8] & ~(1ll << piecePos);

            BitBoard attacks = this->rankAttackZone[piecePos][rank % ATTACK_MODULO + ATTACK_MODULO] & ~this->whitePiece;
            BitBoard attack = attacks & ~(attacks - 1);

            this->whiteRookAttackZones |= attacks;

            while (attack)
            {
                int attackPos = this->log2[attack % LOG2_MODULO + LOG2_MODULO];

                this->whiteMoves.emplace_back();
                this->whiteMoves.back() = this->whiteMoves.back() + 'R' + toChar(piecePos / 8) + toChar(piecePos % 8) + toChar(attackPos / 8) + toChar(attackPos % 8) + ' ';

                attacks &= ~attack;
                attack = attacks & ~(attacks - 1);
            }

            BitBoard file = this->occupied & this->file[piecePos % 8] & ~(1ll << piecePos);

            attacks = this->fileAttackZone[piecePos][file % ATTACK_MODULO + ATTACK_MODULO] & ~this->whitePiece;
            attack = attacks & ~(attacks - 1);

            this->whiteRookAttackZones |= attacks;

            while (attack)
            {
                int attackPos = this->log2[attack % LOG2_MODULO + LOG2_MODULO];

                this->whiteMoves.emplace_back();
                this->whiteMoves.back() = this->whiteMoves.back() + 'R' + toChar(piecePos / 8) + toChar(piecePos % 8) + toChar(attackPos / 8) + toChar(attackPos % 8) + ' ';

                attacks &= ~attack;
                attack = attacks & ~(attacks - 1);
            }

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }
    }

    void generateWhiteKnightMoves()
    {
        this->whiteKnightAttackZones = 0ll;

        BitBoard pieces;
        BitBoard piece;

        pieces = this->whiteKnight;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int piecePos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            BitBoard attacks = (this->knightAttackZone[piecePos] & ~this->whitePiece);
            BitBoard attack = attacks & ~(attacks - 1);

            this->whiteKnightAttackZones |= attacks;

            while (attack)
            {
                int attackPos = this->log2[attack % LOG2_MODULO + LOG2_MODULO];

                this->whiteMoves.emplace_back();
                this->whiteMoves.back() = this->whiteMoves.back() + 'N' + toChar(piecePos / 8) + toChar(piecePos % 8) + toChar(attackPos / 8) + toChar(attackPos % 8) + ' ';

                attacks &= ~attack;
                attack = attacks & ~(attacks - 1);
            }

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }
    }

    void generateWhiteBishopMoves()
    {
        this->whiteBishopAttackZones = 0ll;

        BitBoard pieces;
        BitBoard piece;

        pieces = this->whiteBishop;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int piecePos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            BitBoard diagonal1 = this->occupied & this->diagonal1[7 - piecePos % 8 + piecePos / 8] & ~(1ll << piecePos);

            BitBoard attacks = this->diagonal1AttackZone[piecePos][diagonal1 % ATTACK_MODULO + ATTACK_MODULO] & ~this->whitePiece;
            BitBoard attack = attacks & ~(attacks - 1);

            this->whiteBishopAttackZones |= attacks;

            while (attack)
            {
                int attackPos = this->log2[attack % LOG2_MODULO + LOG2_MODULO];

                this->whiteMoves.emplace_back();
                this->whiteMoves.back() = this->whiteMoves.back() + 'B' + toChar(piecePos / 8) + toChar(piecePos % 8) + toChar(attackPos / 8) + toChar(attackPos % 8) + ' ';


                attacks &= ~attack;
                attack = attacks & ~(attacks - 1);
            }

            BitBoard diagonal2 = this->occupied & this->diagonal2[7 - piecePos / 8 + 7 - piecePos % 8] & ~(1ll << piecePos);

            attacks = this->diagonal2AttackZone[piecePos][diagonal2 % ATTACK_MODULO + ATTACK_MODULO] & ~this->whitePiece;
            attack = attacks & ~(attacks - 1);

            this->whiteBishopAttackZones |= attacks;

            while (attack)
            {
                int attackPos = this->log2[attack % LOG2_MODULO + LOG2_MODULO];

                this->whiteMoves.emplace_back();
                this->whiteMoves.back() = this->whiteMoves.back() + 'B' + toChar(piecePos / 8) + toChar(piecePos % 8) + toChar(attackPos / 8) + toChar(attackPos % 8) + ' ';

                attacks &= ~attack;
                attack = attacks & ~(attacks - 1);
            }

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }
    }

    void generateWhiteQueenMoves()
    {
        this->whiteQueenAttackZones = 0ll;

        BitBoard pieces;
        BitBoard piece;

        pieces = this->whiteQueen;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int piecePos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            BitBoard rank = this->occupied & this->rank[piecePos / 8] & ~(1ll << piecePos);

            BitBoard attacks = this->rankAttackZone[piecePos][rank % ATTACK_MODULO + ATTACK_MODULO] & ~this->whitePiece;
            BitBoard attack = attacks & ~(attacks - 1);

            this->whiteQueenAttackZones |= attacks;

            while (attack)
            {
                int attackPos = this->log2[attack % LOG2_MODULO + LOG2_MODULO];

                this->whiteMoves.emplace_back();
                this->whiteMoves.back() = this->whiteMoves.back() + 'Q' + toChar(piecePos / 8) + toChar(piecePos % 8) + toChar(attackPos / 8) + toChar(attackPos % 8) + ' ';

                attacks &= ~attack;
                attack = attacks & ~(attacks - 1);
            }

            BitBoard file = this->occupied & this->file[piecePos % 8] & ~(1ll << piecePos);

            attacks = this->fileAttackZone[piecePos][file % ATTACK_MODULO + ATTACK_MODULO] & ~this->whitePiece;
            attack = attacks & ~(attacks - 1);

            this->whiteQueenAttackZones |= attacks;

            while (attack)
            {
                int attackPos = this->log2[attack % LOG2_MODULO + LOG2_MODULO];

                this->whiteMoves.emplace_back();
                this->whiteMoves.back() = this->whiteMoves.back() + 'Q' + toChar(piecePos / 8) + toChar(piecePos % 8) + toChar(attackPos / 8) + toChar(attackPos % 8) + ' ';

                attacks &= ~attack;
                attack = attacks & ~(attacks - 1);
            }

            BitBoard diagonal1 = this->occupied & this->diagonal1[7 - piecePos % 8 + piecePos / 8] & ~(1ll << piecePos);

            attacks = this->diagonal1AttackZone[piecePos][diagonal1 % ATTACK_MODULO + ATTACK_MODULO] & ~this->whitePiece;
            attack = attacks & ~(attacks - 1);

            this->whiteQueenAttackZones |= attacks;

            while (attack)
            {
                int attackPos = this->log2[attack % LOG2_MODULO + LOG2_MODULO];

                this->whiteMoves.emplace_back();
                this->whiteMoves.back() = this->whiteMoves.back() + 'Q' + toChar(piecePos / 8) + toChar(piecePos % 8) + toChar(attackPos / 8) + toChar(attackPos % 8) + ' ';

                attacks &= ~attack;
                attack = attacks & ~(attacks - 1);
            }

            BitBoard diagonal2 = this->occupied & this->diagonal2[7 - piecePos / 8 + 7 - piecePos % 8] & ~(1ll << piecePos);

            attacks = this->diagonal2AttackZone[piecePos][diagonal2 % ATTACK_MODULO + ATTACK_MODULO] & ~this->whitePiece;
            attack = attacks & ~(attacks - 1);

            this->whiteQueenAttackZones |= attacks;

            while (attack)
            {
                int attackPos = this->log2[attack % LOG2_MODULO + LOG2_MODULO];

                this->whiteMoves.emplace_back();
                this->whiteMoves.back() = this->whiteMoves.back() + 'Q' + toChar(piecePos / 8) + toChar(piecePos % 8) + toChar(attackPos / 8) + toChar(attackPos % 8) + ' ';

                attacks &= ~attack;
                attack = attacks & ~(attacks - 1);
            }

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }
    }

    void generateWhiteKingMoves()
    {
        this->whiteKingAttackZones = 0ll;

        BitBoard pieces;
        BitBoard piece;

        pieces = this->whiteKing;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int piecePos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            BitBoard attacks = (this->kingAttackZone[piecePos] & ~this->whitePiece);
            BitBoard attack = attacks & ~(attacks - 1);

            this->whiteKingAttackZones |= attacks;

            while (attack)
            {
                int attackPos = this->log2[attack % LOG2_MODULO + LOG2_MODULO];

                this->whiteMoves.emplace_back();
                this->whiteMoves.back() = this->whiteMoves.back() + 'K' + toChar(piecePos / 8) + toChar(piecePos % 8) + toChar(attackPos / 8) + toChar(attackPos % 8) + ' ';

                attacks &= ~attack;
                attack = attacks & ~(attacks - 1);
            }

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }
    }

    void generateBlackPawnMoves()
    {
        this->blackPawnAttackZones = 0ll;

        /// Toate reperele sunt fata de matricea din afisare, cum o vedem noi.

        /// Atac stanga

        BitBoard pieces = this->blackPawn & (this->whitePiece << 9) & ~this->rank[1] & ~this->file[0];
        BitBoard piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            this->blackPawnAttackZones |= (1ll << (pos - 9));
            this->blackMoves.emplace_back();
            this->blackMoves.back() = this->blackMoves.back() + 'p' + toChar(pos / 8) + toChar(pos % 8) + toChar(pos / 8 - 1) + toChar(pos % 8 - 1) + ' ';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        /// Atac dreapta

        pieces = this->blackPawn & (this->whitePiece << 7) & ~this->rank[1] & ~this->file[7];
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            this->blackPawnAttackZones |= (1ll << (pos - 7));
            this->blackMoves.emplace_back();
            this->blackMoves.back() = this->blackMoves.back() + 'p' + toChar(pos / 8) + toChar(pos % 8) + toChar(pos / 8 - 1) + toChar(pos % 8 + 1) + ' ';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        /// Mers inainte o casuta

        pieces = this->blackPawn & (this->empty << 8) & ~this->rank[1];
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            this->blackMoves.emplace_back();
            this->blackMoves.back() = this->blackMoves.back() + 'p' + toChar(pos / 8) + toChar(pos % 8) + toChar(pos / 8 - 1) + toChar(pos % 8) + ' ';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        /// Mers inainte doua casute

        pieces = this->blackPawn & this->rank[6] & (this->empty << 8) & (this->empty << 16);
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            this->blackMoves.emplace_back();
            this->blackMoves.back() = this->blackMoves.back() + 'p' + toChar(pos / 8) + toChar(pos % 8) + toChar(pos / 8 - 2) + toChar(pos % 8) + ' ';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        /// Promovari

        /// Promovare prin atac stanga

        pieces = this->blackPawn & (this->whitePiece << 9) & this->rank[1] & ~this->file[0];
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            this->blackPawnAttackZones |= (1ll << (pos - 9));
            this->blackMoves.emplace_back();
            this->blackMoves.back() = this->blackMoves.back() + 'p' + toChar(pos / 8) + toChar(pos % 8) + toChar(pos / 8 - 1) + toChar(pos % 8 - 1) + 'r';
            this->blackMoves.emplace_back();
            this->blackMoves.back() = this->blackMoves.back() + 'p' + toChar(pos / 8) + toChar(pos % 8) + toChar(pos / 8 - 1) + toChar(pos % 8 - 1) + 'n';
            this->blackMoves.emplace_back();
            this->blackMoves.back() = this->blackMoves.back() + 'p' + toChar(pos / 8) + toChar(pos % 8) + toChar(pos / 8 - 1) + toChar(pos % 8 - 1) + 'b';
            this->blackMoves.emplace_back();
            this->blackMoves.back() = this->blackMoves.back() + 'p' + toChar(pos / 8) + toChar(pos % 8) + toChar(pos / 8 - 1) + toChar(pos % 8 - 1) + 'q';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        /// Promovare prin atac dreapta

        pieces = this->blackPawn & (this->whitePiece << 7) & this->rank[1] & ~this->file[7];
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            this->blackPawnAttackZones |= (1ll << (pos - 7));
            this->blackMoves.emplace_back();
            this->blackMoves.back() = this->blackMoves.back() + 'p' + toChar(pos / 8) + toChar(pos % 8) + toChar(pos / 8 - 1) + toChar(pos % 8 + 1) + 'r';
            this->blackMoves.emplace_back();
            this->blackMoves.back() = this->blackMoves.back() + 'p' + toChar(pos / 8) + toChar(pos % 8) + toChar(pos / 8 - 1) + toChar(pos % 8 + 1) + 'n';
            this->blackMoves.emplace_back();
            this->blackMoves.back() = this->blackMoves.back() + 'p' + toChar(pos / 8) + toChar(pos % 8) + toChar(pos / 8 - 1) + toChar(pos % 8 + 1) + 'b';
            this->blackMoves.emplace_back();
            this->blackMoves.back() = this->blackMoves.back() + 'p' + toChar(pos / 8) + toChar(pos % 8) + toChar(pos / 8 - 1) + toChar(pos % 8 + 1) + 'q';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        /// Promovare prin mers inainte o casuta

        pieces = this->blackPawn & (this->empty << 8) & this->rank[1];
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            this->blackMoves.emplace_back();
            this->blackMoves.back() = this->blackMoves.back() + 'p' + toChar(pos / 8) + toChar(pos % 8) + toChar(pos / 8 - 1) + toChar(pos % 8) + 'r';
            this->blackMoves.emplace_back();
            this->blackMoves.back() = this->blackMoves.back() + 'p' + toChar(pos / 8) + toChar(pos % 8) + toChar(pos / 8 - 1) + toChar(pos % 8) + 'n';
            this->blackMoves.emplace_back();
            this->blackMoves.back() = this->blackMoves.back() + 'p' + toChar(pos / 8) + toChar(pos % 8) + toChar(pos / 8 - 1) + toChar(pos % 8) + 'b';
            this->blackMoves.emplace_back();
            this->blackMoves.back() = this->blackMoves.back() + 'p' + toChar(pos / 8) + toChar(pos % 8) + toChar(pos / 8 - 1) + toChar(pos % 8) + 'q';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        /// En passant

        /// Atac stanga

        pieces = this->blackPawn & (enPassant << 1) & this->rank[3];
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            this->blackPawnAttackZones |= (1ll << (pos - 1));
            this->blackMoves.emplace_back();
            this->blackMoves.back() = this->blackMoves.back() + 'p' + toChar(pos / 8) + toChar(pos % 8) + toChar(pos / 8 - 1) + toChar(pos % 8 - 1) + '+';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        /// Atac dreapta

        pieces = (this->blackPawn << 1) & enPassant & this->rank[3];
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            this->blackPawnAttackZones |= (1ll << pos);
            this->blackMoves.emplace_back();
            this->blackMoves.back() = this->blackMoves.back() + 'p' + toChar(pos / 8) + toChar(pos % 8 - 1) + toChar(pos / 8 - 1) + toChar(pos % 8) + '+';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }
    }

    void generateBlackRookMoves()
    {
        this->blackRookAttackZones = 0ll;

        BitBoard pieces;
        BitBoard piece;

        pieces = this->blackRook;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int piecePos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            BitBoard rank = this->occupied & this->rank[piecePos / 8] & ~(1ll << piecePos);

            BitBoard attacks = this->rankAttackZone[piecePos][rank % ATTACK_MODULO + ATTACK_MODULO] & ~this->blackPiece;
            BitBoard attack = attacks & ~(attacks - 1);

            this->blackRookAttackZones |= attacks;

            while (attack)
            {
                int attackPos = this->log2[attack % LOG2_MODULO + LOG2_MODULO];

                this->blackMoves.emplace_back();
                this->blackMoves.back() = this->blackMoves.back() + 'r' + toChar(piecePos / 8) + toChar(piecePos % 8) + toChar(attackPos / 8) + toChar(attackPos % 8) + ' ';

                attacks &= ~attack;
                attack = attacks & ~(attacks - 1);
            }

            BitBoard file = this->occupied & this->file[piecePos % 8] & ~(1ll << piecePos);

            attacks = this->fileAttackZone[piecePos][file % ATTACK_MODULO + ATTACK_MODULO] & ~this->blackPiece;
            attack = attacks & ~(attacks - 1);

            this->blackRookAttackZones |= attacks;

            while (attack)
            {
                int attackPos = this->log2[attack % LOG2_MODULO + LOG2_MODULO];

                this->blackMoves.emplace_back();
                this->blackMoves.back() = this->blackMoves.back() + 'r' + toChar(piecePos / 8) + toChar(piecePos % 8) + toChar(attackPos / 8) + toChar(attackPos % 8) + ' ';

                attacks &= ~attack;
                attack = attacks & ~(attacks - 1);
            }

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }
    }

    void generateBlackKnightMoves()
    {
        this->blackKnightAttackZones = 0ll;

        BitBoard pieces;
        BitBoard piece;

        pieces = this->blackKnight;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int piecePos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            BitBoard attacks = (this->knightAttackZone[piecePos] & ~this->blackPiece);
            BitBoard attack = attacks & ~(attacks - 1);

            this->blackKnightAttackZones |= attacks;

            while (attack)
            {
                int attackPos = this->log2[attack % LOG2_MODULO + LOG2_MODULO];

                this->blackMoves.emplace_back();
                this->blackMoves.back() = this->blackMoves.back() + 'n' + toChar(piecePos / 8) + toChar(piecePos % 8) + toChar(attackPos / 8) + toChar(attackPos % 8) + ' ';

                attacks &= ~attack;
                attack = attacks & ~(attacks - 1);
            }

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }
    }

    void generateBlackBishopMoves()
    {
        this->blackBishopAttackZones = 0ll;

        BitBoard pieces;
        BitBoard piece;

        pieces = this->blackBishop;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int piecePos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            BitBoard diagonal1 = this->occupied & this->diagonal1[7 - piecePos % 8 + piecePos / 8] & ~(1ll << piecePos);

            BitBoard attacks = this->diagonal1AttackZone[piecePos][diagonal1 % ATTACK_MODULO + ATTACK_MODULO] & ~this->blackPiece;
            BitBoard attack = attacks & ~(attacks - 1);

            this->blackBishopAttackZones |= attacks;

            while (attack)
            {
                int attackPos = this->log2[attack % LOG2_MODULO + LOG2_MODULO];

                this->blackMoves.emplace_back();
                this->blackMoves.back() = this->blackMoves.back() + 'b' + toChar(piecePos / 8) + toChar(piecePos % 8) + toChar(attackPos / 8) + toChar(attackPos % 8) + ' ';

                attacks &= ~attack;
                attack = attacks & ~(attacks - 1);
            }

            BitBoard diagonal2 = this->occupied & this->diagonal2[7 - piecePos / 8 + 7 - piecePos % 8] & ~(1ll << piecePos);

            attacks = this->diagonal2AttackZone[piecePos][diagonal2 % ATTACK_MODULO + ATTACK_MODULO] & ~this->blackPiece;
            attack = attacks & ~(attacks - 1);

            this->blackBishopAttackZones |= attacks;

            while (attack)
            {
                int attackPos = this->log2[attack % LOG2_MODULO + LOG2_MODULO];

                this->blackMoves.emplace_back();
                this->blackMoves.back() = this->blackMoves.back() + 'b' + toChar(piecePos / 8) + toChar(piecePos % 8) + toChar(attackPos / 8) + toChar(attackPos % 8) + ' ';

                attacks &= ~attack;
                attack = attacks & ~(attacks - 1);
            }

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }
    }

    void generateBlackQueenMoves()
    {
        this->blackQueenAttackZones = 0ll;

        BitBoard pieces;
        BitBoard piece;

        pieces = this->blackQueen;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int piecePos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            BitBoard rank = this->occupied & this->rank[piecePos / 8] & ~(1ll << piecePos);

            BitBoard attacks = this->rankAttackZone[piecePos][rank % ATTACK_MODULO + ATTACK_MODULO] & ~this->blackPiece;
            BitBoard attack = attacks & ~(attacks - 1);

            this->blackQueenAttackZones |= attacks;

            while (attack)
            {
                int attackPos = this->log2[attack % LOG2_MODULO + LOG2_MODULO];

                this->blackMoves.emplace_back();
                this->blackMoves.back() = this->blackMoves.back() + 'q' + toChar(piecePos / 8) + toChar(piecePos % 8) + toChar(attackPos / 8) + toChar(attackPos % 8) + ' ';

                attacks &= ~attack;
                attack = attacks & ~(attacks - 1);
            }

            BitBoard file = this->occupied & this->file[piecePos % 8] & ~(1ll << piecePos);

            attacks = this->fileAttackZone[piecePos][file % ATTACK_MODULO + ATTACK_MODULO] & ~this->blackPiece;
            attack = attacks & ~(attacks - 1);

            this->blackQueenAttackZones |= attacks;

            while (attack)
            {
                int attackPos = this->log2[attack % LOG2_MODULO + LOG2_MODULO];

                this->blackMoves.emplace_back();
                this->blackMoves.back() = this->blackMoves.back() + 'q' + toChar(piecePos / 8) + toChar(piecePos % 8) + toChar(attackPos / 8) + toChar(attackPos % 8) + ' ';

                attacks &= ~attack;
                attack = attacks & ~(attacks - 1);
            }

            BitBoard diagonal1 = this->occupied & this->diagonal1[7 - piecePos % 8 + piecePos / 8] & ~(1ll << piecePos);

            attacks = this->diagonal1AttackZone[piecePos][diagonal1 % ATTACK_MODULO + ATTACK_MODULO] & ~this->blackPiece;
            attack = attacks & ~(attacks - 1);

            this->blackQueenAttackZones |= attacks;

            while (attack)
            {
                int attackPos = this->log2[attack % LOG2_MODULO + LOG2_MODULO];

                this->blackMoves.emplace_back();
                this->blackMoves.back() = this->blackMoves.back() + 'q' + toChar(piecePos / 8) + toChar(piecePos % 8) + toChar(attackPos / 8) + toChar(attackPos % 8) + ' ';

                attacks &= ~attack;
                attack = attacks & ~(attacks - 1);
            }

            BitBoard diagonal2 = this->occupied & this->diagonal2[7 - piecePos / 8 + 7 - piecePos % 8] & ~(1ll << piecePos);

            attacks = this->diagonal2AttackZone[piecePos][diagonal2 % ATTACK_MODULO + ATTACK_MODULO] & ~this->blackPiece;
            attack = attacks & ~(attacks - 1);

            this->blackQueenAttackZones |= attacks;

            while (attack)
            {
                int attackPos = this->log2[attack % LOG2_MODULO + LOG2_MODULO];

                this->blackMoves.emplace_back();
                this->blackMoves.back() = this->blackMoves.back() + 'q' + toChar(piecePos / 8) + toChar(piecePos % 8) + toChar(attackPos / 8) + toChar(attackPos % 8) + ' ';

                attacks &= ~attack;
                attack = attacks & ~(attacks - 1);
            }

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }
    }

    void generateBlackKingMoves()
    {
        this->blackKingAttackZones = 0ll;

        BitBoard pieces;
        BitBoard piece;

        pieces = this->blackKing;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int piecePos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            BitBoard attacks = (this->kingAttackZone[piecePos] & ~this->blackPiece);
            BitBoard attack = attacks & ~(attacks - 1);

            this->blackKingAttackZones |= attacks;

            while (attack)
            {
                int attackPos = this->log2[attack % LOG2_MODULO + LOG2_MODULO];

                this->blackMoves.emplace_back();
                this->blackMoves.back() = this->blackMoves.back() + 'k' + toChar(piecePos / 8) + toChar(piecePos % 8) + toChar(attackPos / 8) + toChar(attackPos % 8) + ' ';

                attacks &= ~attack;
                attack = attacks & ~(attacks - 1);
            }

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }
    }

    void generateWhitePawnAttackZones()
    {
        this->whitePawnAttackZones = 0ll;

        BitBoard pieces;

        /// Toate reperele sunt fata de matricea din afisare, cum o vedem noi.

        /// Atac stanga

        pieces = (this->whitePawn << 7) & this->blackPiece & ~this->file[7];
        this->whitePawnAttackZones |= pieces;

        /// Atac dreapta

        pieces = (this->whitePawn << 9) & this->blackPiece & ~this->file[0];
        this->whitePawnAttackZones |= pieces;

        /// En passant

        /// Atac stanga

        pieces = this->whitePawn & (this->enPassant << 1) & this->rank[4];
        this->whitePawnAttackZones |= pieces;

        /// Atac dreapta

        pieces = (this->whitePawn << 1) & this->enPassant & this->rank[4];
        this->whitePawnAttackZones |= pieces;
    }

    void generateWhiteRookAttackZones()
    {
        this->whiteRookAttackZones = 0ll;

        BitBoard pieces;
        BitBoard piece;

        pieces = this->whiteRook;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int piecePos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            BitBoard rank = this->occupied & this->rank[piecePos / 8] & ~(1ll << piecePos);
            BitBoard attacks = this->rankAttackZone[piecePos][rank % ATTACK_MODULO + ATTACK_MODULO] & ~this->whitePiece;
            this->whiteRookAttackZones |= attacks;

            BitBoard file = this->occupied & this->file[piecePos % 8] & ~(1ll << piecePos);
            attacks = this->fileAttackZone[piecePos][file % ATTACK_MODULO + ATTACK_MODULO] & ~this->whitePiece;
            this->whiteRookAttackZones |= attacks;

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }
    }

    void generateWhiteKnightAttackZones()
    {
        this->whiteKnightAttackZones = 0ll;

        BitBoard pieces;
        BitBoard piece;

        pieces = this->whiteKnight;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int piecePos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            BitBoard attacks = this->knightAttackZone[piecePos] & ~this->whitePiece;
            this->whiteKnightAttackZones |= attacks;

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }
    }

    void generateWhiteBishopAttackZones()
    {
        this->whiteBishopAttackZones = 0ll;

        BitBoard pieces;
        BitBoard piece;

        pieces = this->whiteBishop;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int piecePos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            BitBoard diagonal1 = this->occupied & this->diagonal1[7 - piecePos % 8 + piecePos / 8] & ~(1ll << piecePos);
            BitBoard attacks = this->diagonal1AttackZone[piecePos][diagonal1 % ATTACK_MODULO + ATTACK_MODULO] & ~this->whitePiece;
            this->whiteBishopAttackZones |= attacks;

            BitBoard diagonal2 = this->occupied & this->diagonal2[7 - piecePos / 8 + 7 - piecePos % 8] & ~(1ll << piecePos);
            attacks = this->diagonal2AttackZone[piecePos][diagonal2 % ATTACK_MODULO + ATTACK_MODULO] & ~this->whitePiece;
            this->whiteBishopAttackZones |= attacks;

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }
    }

    void generateWhiteQueenAttackZones()
    {
        this->whiteQueenAttackZones = 0ll;

        BitBoard pieces;
        BitBoard piece;

        pieces = this->whiteQueen;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int piecePos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            BitBoard rank = this->occupied & this->rank[piecePos / 8] & ~(1ll << piecePos);
            BitBoard attacks = this->rankAttackZone[piecePos][rank % ATTACK_MODULO + ATTACK_MODULO] & ~this->whitePiece;
            this->whiteQueenAttackZones |= attacks;

            BitBoard file = this->occupied & this->file[piecePos % 8] & ~(1ll << piecePos);
            attacks = this->fileAttackZone[piecePos][file % ATTACK_MODULO + ATTACK_MODULO] & ~this->whitePiece;
            this->whiteQueenAttackZones |= attacks;

            BitBoard diagonal1 = this->occupied & this->diagonal1[7 - piecePos % 8 + piecePos / 8] & ~(1ll << piecePos);
            attacks = this->diagonal1AttackZone[piecePos][diagonal1 % ATTACK_MODULO + ATTACK_MODULO] & ~this->whitePiece;
            this->whiteQueenAttackZones |= attacks;

            BitBoard diagonal2 = this->occupied & this->diagonal2[7 - piecePos / 8 + 7 - piecePos % 8] & ~(1ll << piecePos);
            attacks = this->diagonal2AttackZone[piecePos][diagonal2 % ATTACK_MODULO + ATTACK_MODULO] & ~this->whitePiece;
            this->whiteQueenAttackZones |= attacks;

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }
    }

    void generateWhiteKingAttackZones()
    {
        this->whiteKingAttackZones = 0ll;

        BitBoard pieces;
        BitBoard piece;

        pieces = this->whiteKing;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int piecePos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            BitBoard attacks = (this->kingAttackZone[piecePos] & ~this->whitePiece);
            this->whiteKingAttackZones |= attacks;

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }
    }

    void generateBlackPawnAttackZones()
    {
        this->blackPawnAttackZones = 0ll;

        BitBoard pieces;

        /// Toate reperele sunt fata de matricea din afisare, cum o vedem noi.

        /// Atac stanga

        pieces = this->blackPawn & (this->whitePiece << 9) & ~this->file[0];
        this->blackPawnAttackZones |= pieces;

        /// Atac dreapta

        pieces = this->blackPawn & (this->whitePiece << 7) & ~this->file[7];
        this->blackPawnAttackZones |= pieces;

        /// En passant

        /// Atac stanga

        pieces = this->blackPawn & (enPassant << 1) & this->rank[3];
        this->blackPawnAttackZones |= pieces;

        /// Atac dreapta

        pieces = (this->blackPawn << 1) & enPassant & this->rank[3];
        this->blackPawnAttackZones |= pieces;
    }

    void generateBlackRookAttackZones()
    {
        this->blackRookAttackZones = 0ll;

        BitBoard pieces;
        BitBoard piece;

        pieces = this->blackRook;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int piecePos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            BitBoard rank = this->occupied & this->rank[piecePos / 8] & ~(1ll << piecePos);
            BitBoard attacks = this->rankAttackZone[piecePos][rank % ATTACK_MODULO + ATTACK_MODULO] & ~this->blackPiece;
            this->blackRookAttackZones |= attacks;

            BitBoard file = this->occupied & this->file[piecePos % 8] & ~(1ll << piecePos);
            attacks = this->fileAttackZone[piecePos][file % ATTACK_MODULO + ATTACK_MODULO] & ~this->blackPiece;
            this->blackRookAttackZones |= attacks;

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }
    }

    void generateBlackKnightAttackZones()
    {
        this->blackKnightAttackZones = 0ll;

        BitBoard pieces;
        BitBoard piece;

        pieces = this->blackKnight;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int piecePos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            BitBoard attacks = (this->knightAttackZone[piecePos] & ~this->blackPiece);
            this->blackKnightAttackZones |= attacks;

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }
    }

    void generateBlackBishopAttackZones()
    {
        this->blackBishopAttackZones = 0ll;

        BitBoard pieces;
        BitBoard piece;

        pieces = this->blackBishop;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int piecePos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            BitBoard diagonal1 = this->occupied & this->diagonal1[7 - piecePos % 8 + piecePos / 8] & ~(1ll << piecePos);
            BitBoard attacks = this->diagonal1AttackZone[piecePos][diagonal1 % ATTACK_MODULO + ATTACK_MODULO] & ~this->blackPiece;
            this->blackBishopAttackZones |= attacks;

            BitBoard diagonal2 = this->occupied & this->diagonal2[7 - piecePos / 8 + 7 - piecePos % 8] & ~(1ll << piecePos);
            attacks = this->diagonal2AttackZone[piecePos][diagonal2 % ATTACK_MODULO + ATTACK_MODULO] & ~this->blackPiece;
            this->blackBishopAttackZones |= attacks;

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }
    }

    void generateBlackQueenAttackZones()
    {
        this->blackQueenAttackZones = 0ll;

        BitBoard pieces;
        BitBoard piece;

        pieces = this->blackQueen;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int piecePos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            BitBoard rank = this->occupied & this->rank[piecePos / 8] & ~(1ll << piecePos);
            BitBoard attacks = this->rankAttackZone[piecePos][rank % ATTACK_MODULO + ATTACK_MODULO] & ~this->blackPiece;
            this->blackQueenAttackZones |= attacks;

            BitBoard file = this->occupied & this->file[piecePos % 8] & ~(1ll << piecePos);
            attacks = this->fileAttackZone[piecePos][file % ATTACK_MODULO + ATTACK_MODULO] & ~this->blackPiece;
            this->blackQueenAttackZones |= attacks;

            BitBoard diagonal1 = this->occupied & this->diagonal1[7 - piecePos % 8 + piecePos / 8] & ~(1ll << piecePos);
            attacks = this->diagonal1AttackZone[piecePos][diagonal1 % ATTACK_MODULO + ATTACK_MODULO] & ~this->blackPiece;
            this->blackQueenAttackZones |= attacks;

            BitBoard diagonal2 = this->occupied & this->diagonal2[7 - piecePos / 8 + 7 - piecePos % 8] & ~(1ll << piecePos);
            attacks = this->diagonal2AttackZone[piecePos][diagonal2 % ATTACK_MODULO + ATTACK_MODULO] & ~this->blackPiece;
            this->blackQueenAttackZones |= attacks;

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }
    }

    void generateBlackKingAttackZones()
    {
        this->blackKingAttackZones = 0ll;

        BitBoard pieces;
        BitBoard piece;

        pieces = this->blackKing;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int piecePos = this->log2[piece % LOG2_MODULO + LOG2_MODULO];

            BitBoard attacks = (this->kingAttackZone[piecePos] & ~this->blackPiece);
            this->blackKingAttackZones |= attacks;

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }
    }

    inline bool isWhiteKingInCheck() const
    {
        return this->whiteKing & this->blackPieceAttackZones;
    }

    inline bool isBlackKingInCheck() const
    {
        return this->blackKing & this->whitePieceAttackZones;
    }

    void generateWhiteMoves()
    {
        this->whiteMoves.clear();

        /// Generam mutarile in ordinea asta ca sa aveam alea pentru piesele mai importante primele, incat poate taiem mai mult la alpha-beta asa.
        this->generateWhiteKnightMoves();
        this->generateWhiteQueenMoves();
        this->generateWhiteRookMoves();
        this->generateWhiteBishopMoves();
        this->generateWhitePawnMoves();
        this->generateWhiteKingMoves();

        this->whitePieceAttackZones = this->whitePawnAttackZones | this->whiteRookAttackZones | this->whiteKnightAttackZones |
            this->whiteBishopAttackZones | this->whiteQueenAttackZones | this->whiteKingAttackZones;

        if (!this->isWhiteKingInCheck() && !this->whiteKingMoved)
        {
            if (!this->whiteLeftRookMoved && !((this->whiteLeftSide & this->occupied) || (this->whiteLeftSide & this->blackPieceAttackZones)))
            {
                this->whiteMoves.emplace_back();
                this->whiteMoves.back() = this->whiteMoves.back() + 'K' + toChar(0) + toChar(3) + toChar(0) + toChar(1) + ' ';
            }
            if (!this->whiteRightRookMoved && !((this->whiteRightSide & this->blackPieceAttackZones) | (this->whiteExtendedRightSide & this->occupied)))
            {
                this->whiteMoves.emplace_back();
                this->whiteMoves.back() = this->whiteMoves.back() + 'K' + toChar(0) + toChar(3) + toChar(0) + toChar(5) + ' ';
            }
        }
    }

    void generateBlackMoves()
    {
        this->blackMoves.clear();

        /// Generam mutarile in ordinea asta ca sa aveam alea pentru piesele mai importante primele, incat poate taiem mai mult la alpha-beta asa.
        this->generateBlackKnightMoves();
        this->generateBlackQueenMoves();
        this->generateBlackRookMoves();
        this->generateBlackBishopMoves();
        this->generateBlackPawnMoves();
        this->generateBlackKingMoves();

        this->blackPieceAttackZones = this->blackPawnAttackZones | this->blackRookAttackZones | this->blackKnightAttackZones |
            this->blackBishopAttackZones | this->blackQueenAttackZones | this->blackKingAttackZones;

        if (!this->isBlackKingInCheck() && !this->blackKingMoved)
        {
            if (!this->blackLeftRookMoved && !((this->blackLeftSide & this->occupied) || (this->blackLeftSide & this->whitePieceAttackZones)))
            {
                this->blackMoves.emplace_back();
                this->blackMoves.back() = this->blackMoves.back() + 'k' + toChar(7) + toChar(3) + toChar(7) + toChar(1) + ' ';
            }
            if (!this->blackRightRookMoved && !((this->blackRightSide & this->whitePieceAttackZones) | (this->blackExtendedRightSide & this->occupied)))
            {
                this->blackMoves.emplace_back();
                this->blackMoves.back() = this->blackMoves.back() + 'k' + toChar(7) + toChar(3) + toChar(7) + toChar(5) + ' ';
            }
        }
    }

    void generateWhiteAttackZones()
    {
        this->generateWhitePawnAttackZones();
        this->generateWhiteRookAttackZones();
        this->generateWhiteKnightAttackZones();
        this->generateWhiteBishopAttackZones();
        this->generateWhiteQueenAttackZones();
        this->generateWhiteKingAttackZones();

        this->whitePieceAttackZones = this->whitePawnAttackZones | this->whiteRookAttackZones | this->whiteKnightAttackZones |
            this->whiteBishopAttackZones | this->whiteQueenAttackZones | this->whiteKingAttackZones;
    }

    void generateBlackAttackZones()
    {
        this->generateBlackPawnAttackZones();
        this->generateBlackRookAttackZones();
        this->generateBlackKnightAttackZones();
        this->generateBlackBishopAttackZones();
        this->generateBlackQueenAttackZones();
        this->generateBlackKingAttackZones();

        this->blackPieceAttackZones = this->blackPawnAttackZones | this->blackRookAttackZones | this->blackKnightAttackZones |
            this->blackBishopAttackZones | this->blackQueenAttackZones | this->blackKingAttackZones;
    }

    void doWhiteMove(std::string& move)
    {
        createPrevStatus();
        this->movesHistory.emplace_back(move);

        this->enPassant = 0ll;

        if (move[0] == 'K')
        {
            this->whiteKing &= ~(1ll << ((move[1] - '0') * 8 + move[2] - '0'));
            this->whiteKing |= (1ll << ((move[3] - '0') * 8 + move[4] - '0'));

            this->whiteKingMoved = true;

            if (move[1] == '0' && move[2] == '3')
            {
                if (move[3] == '0' && move[4] == '1')
                {
                    this->whiteRook &= ~(1ll << whiteLeftRook);
                    this->whiteRook |= (1ll << 2);

                    this->whiteLeftRookMoved = true;

                    this->whiteKingCastled = true;

                    this->whitePiece = this->whitePawn | this->whiteRook | this->whiteKnight | this->whiteBishop | this->whiteQueen | this->whiteKing;
                    this->occupied = this->whitePiece | this->blackPiece;
                    this->empty = ~this->occupied;

                    return;
                }
                else if (move[3] == '0' && move[4] == '5')
                {
                    this->whiteRook &= ~(1ll << whiteRightRook);
                    this->whiteRook |= (1ll << 4);

                    this->whiteRightRookMoved = true;

                    this->whiteKingCastled = true;

                    this->whitePiece = this->whitePawn | this->whiteRook | this->whiteKnight | this->whiteBishop | this->whiteQueen | this->whiteKing;
                    this->occupied = this->whitePiece | this->blackPiece;
                    this->empty = ~this->occupied;

                    return;
                }
            }
        }
        else if (move[0] == 'Q')
        {
            this->whiteQueen &= ~(1ll << ((move[1] - '0') * 8 + move[2] - '0'));
            this->whiteQueen |= (1ll << ((move[3] - '0') * 8 + move[4] - '0'));
        }
        else if (move[0] == 'R')
        {
            this->whiteRook &= ~(1ll << ((move[1] - '0') * 8 + move[2] - '0'));
            this->whiteRook |= (1ll << ((move[3] - '0') * 8 + move[4] - '0'));

            if (!this->whiteLeftRookMoved && ((1ll << this->whiteLeftRook) & (1ll << ((move[1] - '0') * 8 + move[2] - '0'))))
                this->whiteLeftRookMoved = true;
            else if (!this->whiteRightRookMoved && ((1ll << this->whiteRightRook) & (1ll << ((move[1] - '0') * 8 + move[2] - '0'))))
                this->whiteRightRookMoved = true;
        }
        else if (move[0] == 'B')
        {
            this->whiteBishop &= ~(1ll << ((move[1] - '0') * 8 + move[2] - '0'));
            this->whiteBishop |= (1ll << ((move[3] - '0') * 8 + move[4] - '0'));
        }
        else if (move[0] == 'N')
        {
            this->whiteKnight &= ~(1ll << ((move[1] - '0') * 8 + move[2] - '0'));
            this->whiteKnight |= (1ll << ((move[3] - '0') * 8 + move[4] - '0'));
        }
        else
        {
            this->whitePawn &= ~(1ll << ((move[1] - '0') * 8 + move[2] - '0'));
            this->whitePawn |= (1ll << ((move[3] - '0') * 8 + move[4] - '0'));

            if (move[3] - move[1] == 2)
                this->enPassant |= (1ll << ((move[3] - '0') * 8 + move[4] - '0'));
        }

        this->blackPawn &= ~(1ll << ((move[3] - '0') * 8 + move[4] - '0'));
        this->blackRook &= ~(1ll << ((move[3] - '0') * 8 + move[4] - '0'));
        this->blackKnight &= ~(1ll << ((move[3] - '0') * 8 + move[4] - '0'));
        this->blackBishop &= ~(1ll << ((move[3] - '0') * 8 + move[4] - '0'));
        this->blackQueen &= ~(1ll << ((move[3] - '0') * 8 + move[4] - '0'));
        this->blackKing &= ~(1ll << ((move[3] - '0') * 8 + move[4] - '0'));

        if (move[5] != ' ')
        {
            if (move[5] == '+')
                this->blackPawn &= ~(1ll << ((move[3] - '0' - 1) * 8 + move[4] - '0'));
            else if (move[5] == 'Q')
            {
                this->whitePawn &= ~(1ll << ((move[3] - '0') * 8 + move[4] - '0'));
                this->whiteQueen |= (1ll << ((move[3] - '0') * 8 + move[4] - '0'));
            }
            else if (move[5] == 'N')
            {
                this->whitePawn &= ~(1ll << ((move[3] - '0') * 8 + move[4] - '0'));
                this->whiteKnight |= (1ll << ((move[3] - '0') * 8 + move[4] - '0'));
            }
            else if (move[5] == 'R')
            {
                this->whitePawn &= ~(1ll << ((move[3] - '0') * 8 + move[4] - '0'));
                this->whiteRook |= (1ll << ((move[3] - '0') * 8 + move[4] - '0'));
            }
            else
            {
                this->whitePawn &= ~(1ll << ((move[3] - '0') * 8 + move[4] - '0'));
                this->whiteBishop |= (1ll << ((move[3] - '0') * 8 + move[4] - '0'));
            }
        }

        this->whitePiece = this->whitePawn | this->whiteRook | this->whiteKnight | this->whiteBishop | this->whiteQueen | this->whiteKing;
        this->blackPiece = this->blackPawn | this->blackRook | this->blackKnight | this->blackBishop | this->blackQueen | this->blackKing;

        this->occupied = this->whitePiece | this->blackPiece;
        this->empty = ~this->occupied;
    }

    void doBlackMove(std::string& move)
    {
        createPrevStatus();
        this->movesHistory.emplace_back(move);

        this->enPassant = 0ll;

        if (move[0] == 'k')
        {
            this->blackKing &= ~(1ll << ((move[1] - '0') * 8 + move[2] - '0'));
            this->blackKing |= (1ll << ((move[3] - '0') * 8 + move[4] - '0'));

            this->blackKingMoved = true;

            if (move[1] == '7' && move[2] == '3')
            {
                if (move[3] == '7' && move[4] == '1')
                {
                    this->blackRook &= ~(1ll << blackLeftRook);
                    this->blackRook |= (1ll << 58);

                    this->blackLeftRookMoved = true;

                    this->blackKingCastled = true;

                    this->blackPiece = this->blackPawn | this->blackRook | this->blackKnight | this->blackBishop | this->blackQueen | this->blackKing;
                    this->occupied = this->whitePiece | this->blackPiece;
                    this->empty = ~this->occupied;

                    return;
                }
                else if (move[3] == '7' && move[4] == '5')
                {
                    this->blackRook &= ~(1ll << blackRightRook);
                    this->blackRook |= (1ll << 60);

                    this->blackRightRookMoved = true;

                    this->blackKingCastled = true;

                    this->blackPiece = this->blackPawn | this->blackRook | this->blackKnight | this->blackBishop | this->blackQueen | this->blackKing;
                    this->occupied = this->whitePiece | this->blackPiece;
                    this->empty = ~this->occupied;

                    return;
                }
            }
        }
        else if (move[0] == 'q')
        {
            this->blackQueen &= ~(1ll << ((move[1] - '0') * 8 + move[2] - '0'));
            this->blackQueen |= (1ll << ((move[3] - '0') * 8 + move[4] - '0'));
        }
        else if (move[0] == 'r')
        {
            this->blackRook &= ~(1ll << ((move[1] - '0') * 8 + move[2] - '0'));
            this->blackRook |= (1ll << ((move[3] - '0') * 8 + move[4] - '0'));

            if (!this->blackLeftRookMoved && ((1ll << this->blackLeftRook) & (1ll << ((move[1] - '0') * 8 + move[2] - '0'))))
                this->blackLeftRookMoved = true;
            else if (!this->blackRightRookMoved && ((1ll << this->blackRightRook) & (1ll << ((move[1] - '0') * 8 + move[2] - '0'))))
                this->blackRightRookMoved = true;
        }
        else if (move[0] == 'b')
        {
            this->blackBishop &= ~(1ll << ((move[1] - '0') * 8 + move[2] - '0'));
            this->blackBishop |= (1ll << ((move[3] - '0') * 8 + move[4] - '0'));
        }
        else if (move[0] == 'n')
        {
            this->blackKnight &= ~(1ll << ((move[1] - '0') * 8 + move[2] - '0'));
            this->blackKnight |= (1ll << ((move[3] - '0') * 8 + move[4] - '0'));
        }
        else
        {
            this->blackPawn &= ~(1ll << ((move[1] - '0') * 8 + move[2] - '0'));
            this->blackPawn |= (1ll << ((move[3] - '0') * 8 + move[4] - '0'));

            if (move[1] - move[3] == 2)
                this->enPassant |= (1ll << ((move[3] - '0') * 8 + move[4] - '0'));
        }

        this->whitePawn &= ~(1ll << ((move[3] - '0') * 8 + move[4] - '0'));
        this->whiteRook &= ~(1ll << ((move[3] - '0') * 8 + move[4] - '0'));
        this->whiteKnight &= ~(1ll << ((move[3] - '0') * 8 + move[4] - '0'));
        this->whiteBishop &= ~(1ll << ((move[3] - '0') * 8 + move[4] - '0'));
        this->whiteQueen &= ~(1ll << ((move[3] - '0') * 8 + move[4] - '0'));
        this->whiteKing &= ~(1ll << ((move[3] - '0') * 8 + move[4] - '0'));

        if (move[5] != ' ')
        {
            if (move[5] == '+')
                this->whitePawn &= ~(1ll << ((move[3] - '0' + 1) * 8 + move[4] - '0'));
            else if (move[5] == 'q')
            {
                this->blackPawn &= ~(1ll << ((move[3] - '0') * 8 + move[4] - '0'));
                this->blackQueen |= (1ll << ((move[3] - '0') * 8 + move[4] - '0'));
            }
            else if (move[5] == 'n')
            {
                this->blackPawn &= ~(1ll << ((move[3] - '0') * 8 + move[4] - '0'));
                this->blackKnight |= (1ll << ((move[3] - '0') * 8 + move[4] - '0'));
            }
            else if (move[5] == 'r')
            {
                this->blackPawn &= ~(1ll << ((move[3] - '0') * 8 + move[4] - '0'));
                this->blackRook |= (1ll << ((move[3] - '0') * 8 + move[4] - '0'));
            }
            else
            {
                this->blackPawn &= ~(1ll << ((move[3] - '0') * 8 + move[4] - '0'));
                this->blackBishop |= (1ll << ((move[3] - '0') * 8 + move[4] - '0'));
            }
        }

        this->whitePiece = this->whitePawn | this->whiteRook | this->whiteKnight | this->whiteBishop | this->whiteQueen | this->whiteKing;
        this->blackPiece = this->blackPawn | this->blackRook | this->blackKnight | this->blackBishop | this->blackQueen | this->blackKing;

        this->occupied = this->whitePiece | this->blackPiece;
        this->empty = ~this->occupied;
    }

    void undoMove()
    {
        this->movesHistory.pop_back();

        loadPrevStatus();

        this->prevStatuses.pop_back();
    }
};

Chess chess;

inline bool compMoves(const std::pair<int, std::string>& a, const std::pair<int, std::string>& b)
{
    return a.first > b.first;
}

class Game
{
public:

    int playerID;
    int AIID;
    int displayMode;

    int MAX_SEARCH_DEPTH;

    static const int WHITE = 1;
    static const int BLACK = 0;

    static const int GAME_CONTINUES = -2;
    static const int DRAW = -1;
    static const int BLACK_WON = 0;
    static const int WHITE_WON = 1;

    static const int INF = 1000000000;
    static const int REACHABLE_INF = 500000000;

    static const int PAWN_SCORE = 50;
    static const int ROOK_SCORE = 275;
    static const int KNIGHT_SCORE = 190;
    static const int BISHOP_SCORE = 215;
    static const int QUEEN_SCORE = 575;
    static const int KING_SCORE = 1000;

    static const int CAN_CASTLE_KING_SCORE = 30;
    static const int CASTLED_KING_SCORE = 50;
    static const int STUPID_CASTLING = 75;

    bool inEndGame = false;

    int whitePawnPositionScore[64] =
    {
         +0,  +0,  +0,  +0,  +0,  +0,  +0,  +0,
         +5, +10, +10, -40, -40, +10, +10,  +5,
         +0, -10, +20,  +0,  +0, +20, -10,  +0,
         +0,  +0,  +0, +25, +25,  +0,  +0,  +0,
         +5,  +5, +25, +35, +35, +25,  +5,  +5,
        +15, +20, +30, +40, +40, +30, +20, +15,
        +50, +50, +50, +50, +50, +50, +50, +50,
        +90, +90, +90, +90, +90, +90, +90, +90,
    };

    int whiteRookPositionScore[64] =
    {
        +10, +10, +10, +15, +15, +10, +10, +10,
        +10, +15,  +5,  +5,  +5,  +5, +15, +10,
         -5,  +0,  +0,  +0,  +0,  +0,  +0,  -5,
         -5,  +0,  +0,  +0,  +0,  +0,  +0,  -5,
         -5,  +0,  +0,  +0,  +0,  +0,  +0,  -5,
         -5,  +0,  +0,  +0,  +0,  +0,  +0,  -5,
        +10, +15, +15, +15, +15, +15, +15, +10,
        +20, +20, +20, +20, +20, +20, +20, +20,
    };

    int whiteKnightPositionScore[64] =
    {
        -50, -40, -30, -20, -20, -30, -40, -50,
        -40, -30,  +0, +10, +10,  +0, -30, -40,
        -30, +10, +15, +25, +25, +15, +10, -30,
        -30, +10, +25, +30, +30, +25, +10, -30,
        -30, +10, +30, +35, +35, +30, +10, -30,
        -30,  +5, +20, +25, +25, +20,  +5, -30,
        -40,  +0,  +0,  +0,  +0,  +0,  +0, -40,
        -50, -40, -30, -30, -30, -30, -40, -50,
    };

    int whiteBishopPositionScore[64] =
    {
        +10, -10, -15, -20, -20, -15, -10, +10,
        -20, +15, -10, -10, -10, -10, +15, -20,
        +20, +20, +20, +10, +10, +20, +20, +20,
        +10, +20, +20, +20, +20, +20, +20, +10,
         +5,  +5,  +5,  +5,  +5,  +5,  +5,  +5,
        +10, +10,  +0,  +0,  +0,  +0, +10, +10,
         +5,  +5,  +5,  +5,  +5,  +5,  +5,  +5,
        +15,  +5,  -5, -10, -10,  -5,  +5, +15,
    };

    int whiteQueenPositionScore[64] =
    {
        -20, -15, -15, -10, -10, -15, -15, -20,
        -15, -10, -10,  -5,  -5, -10, -10, -15,
        -10,  +0, +10, +20, +20, +10,  +0, -10,
         +0, +10, +20, +30, +30, +20, +10,  +0,
         +0, +10, +20, +30, +30, +20, +10,  +0,
        +10, +20, +20, +20, +20, +20, +20, +10,
        +15, +20, +30, +30, +30, +30, +20, +15,
        +20, +30, +40, +40, +40, +40, +30, +20,
    };

    int whiteKingPositionScore[64] =
    {
        +20, +30, +10,  +0,  +0, +10, +30, +20,
        +15, +15,  +0,  +0,  +0,  +0, +15, +15,
        -10, -20, -30, -40, -40, -30, -20, -10,
        -10, -20, -30, -40, -40, -30, -20, -10,
        -30, -40, -50, -60, -60, -50, -40, -30,
        -40, -40, -50, -60, -60, -50, -40, -40,
        -50, -60, -60, -70, -70, -60, -60, -50,
        -60, -70, -70, -80, -80, -70, -70, -60,
    };

    int blackPawnPositionScore[64] =
    {
        +90, +90, +90, +90, +90, +90, +90, +90,
        +50, +50, +50, +50, +50, +50, +50, +50,
        +15, +20, +30, +40, +40, +30, +20, +15,
         +5,  +5, +25, +35, +35, +25,  +5,  +5,
         +0,  +0,  +0, +25, +25,  +0,  +0,  +0,
         +0, -10, +20,  +0,  +0, +20, -10,  +0,
         +5, +10, +10, -40, -40, +10, +10,  +5,
         +0,  +0,  +0,  +0,  +0,  +0,  +0,  +0,
    };

    int blackRookPositionScore[64] =
    {
        +20, +20, +20, +20, +20, +20, +20, +20,
        +10, +15, +15, +15, +15, +15, +15, +10,
         -5,  +0,  +0,  +0,  +0,  +0,  +0,  -5,
         -5,  +0,  +0,  +0,  +0,  +0,  +0,  -5,
         -5,  +0,  +0,  +0,  +0,  +0,  +0,  -5,
         -5,  +0,  +0,  +0,  +0,  +0,  +0,  -5,
        -10, +15,  +5,  +5,  +5,  +5, +15, -10,
        +10, +10, +10, +15, +15, +10, +10, +10,
    };

    int blackKnightPositionScore[64] =
    {
        -50, -40, -30, -30, -30, -30, -40, -50,
        -40,  +0,  +0,  +0,  +0,  +0,  +0, -40,
        -30,  +5, +20, +25, +25, +20,  +5, -30,
        -30, +10, +30, +35, +35, +30, +10, -30,
        -30, +10, +25, +30, +30, +25, +10, -30,
        -30, +10, +15, +25, +25, +15, +10, -30,
        -40, -30,  +0, +10, +10,  +0, -30, -40,
        -50, -40, -30, -20, -20, -30, -40, -50,
    };

    int blackBishopPositionScore[64] =
    {
        +15,  +5,  -5, -10, -10,  -5,  +5, +15,
         +5,  +5,  +5,  +5,  +5,  +5,  +5,  +5,
        +10, +10,  +0,  +0,  +0,  +0, +10, +10,
         +5,  +5,  +5,  +5,  +5,  +5,  +5,  +5,
        +10, +20, +20, +20, +20, +20, +20, +10,
        +20, +20, +20, +10, +10, +20, +20, +20,
        -20, +15, -10, -10, -10, -10, +15, -20,
        +10, -10, -15, -20, -20, -15, -10, +10,
    };

    int blackQueenPositionScore[64] =
    {
        +20, +30, +40, +40, +40, +40, +30, +20,
        +15, +20, +30, +30, +30, +30, +20, +15,
        +10, +20, +20, +20, +20, +20, +20, +10,
         +0, +10, +20, +30, +30, +20, +10,  +0,
         +0, +10, +20, +30, +30, +20, +10,  +0,
        -10,  +0, +10, +20, +20, +10,  +0, -10,
        -15, -10, -10,  -5,  -5, -10, -10, -15,
        -20, -15, -15, -10, -10, -15, -15, -20,
    };

    int blackKingPositionScore[64] =
    {
        -60, -70, -70, -80, -80, -70, -70, -60,
        -50, -60, -60, -70, -70, -60, -60, -50,
        -40, -40, -50, -60, -60, -50, -40, -40,
        -30, -40, -50, -60, -60, -50, -40, -30,
        -20, -40, -50, -60, -60, -50, -40, -20,
        -10, -20, -30, -40, -40, -30, -20, -10,
        +15, +15,  +0,  +0,  +0,  +0, +15, +15,
        +20, +30, +10,  +0,  +0, +10, +30, +20,
    };

    static const int PAWN_PRESSURE_SCORE = 10;
    static const int ROOK_PRESSURE_SCORE = 35;
    static const int KNIGHT_PRESSURE_SCORE = 20;
    static const int BISHOP_PRESSURE_SCORE = 25;
    static const int QUEEN_PRESSURE_SCORE = 45;
    static const int KING_PRESSURE_SCORE = 30;

    int pressureBoard[64];

    int numNodes;

public:

    void initialize()
    {
        std::cout << "Which color is the player? 1 for white, 0 for black." << '\n';
        std::cin >> this->playerID;
        this->AIID = 1 - this->playerID;

        std::cout << "Select the search depth for the A.I.. (Recommended search depth is 4 or 5.)" << '\n';
        std::cin >> MAX_SEARCH_DEPTH;

        std::cout << "From whose perspective will the board be displayed? 1 for white, 0 for black." << '\n';
        std::cin >> this->displayMode;

        system("CLS");
    }

    void transformMove(std::string& move)
    {
        if (move.size() <= 4 || move.size() >= 7)
        {
            if (move.size() >= 1)
                move[0] = '$';
            else
                move += '$';

            return;
        }

        move[1]--;
        move[2] = 'h' - move[2] + '0';

        move[3]--;
        move[4] = 'h' - move[4] + '0';

        if (move.size() == 5)
            move += ' ';
    }

    void announceNextPlayer(int ID)
    {
        if (ID == WHITE)
            std::cout << "White's turn to move!" << '\n';
        else
            std::cout << "Black's turn to move!" << '\n';
    }

    void announceCheck(int ID)
    {
        if (ID == WHITE && chess.isWhiteKingInCheck())
        {
            std::cout << "White in check!" << '\n';
            return;
        }
        else if (ID == BLACK && chess.isBlackKingInCheck())
        {
            std::cout << "Black in check!" << '\n';
            return;
        }
    }

    bool isInEndGame()
    {
        int numWhitePieces = 0;
        int numBlackPieces = 0;

        BitBoard pieces;
        BitBoard piece;

        pieces = chess.whiteRook;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            numWhitePieces++;

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        pieces = chess.whiteBishop;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            numWhitePieces++;

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        pieces = chess.whiteKnight;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            numWhitePieces++;

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        pieces = chess.whiteQueen;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            numWhitePieces++;

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        pieces = chess.blackRook;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            numBlackPieces++;

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        pieces = chess.blackBishop;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            numBlackPieces++;

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        pieces = chess.blackKnight;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            numBlackPieces++;

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        pieces = chess.blackQueen;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            numBlackPieces++;

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        return numWhitePieces <= 2 || numBlackPieces <= 2;
    }

    inline bool isLegalWhiteMove(std::string& move)
    {
        if (move.size() != 6)
            return false;

        int index = -1;

        for (int i = 0; i < chess.whiteMoves.size(); i++)
        {
            if (chess.whiteMoves[i] == move)
            {
                index = i;
                break;
            }
        }

        if (index == -1)
            return false;;

        chess.doWhiteMove(move);
        chess.generateBlackAttackZones();

        if (chess.isWhiteKingInCheck())
        {
            chess.undoMove();
            chess.generateBlackAttackZones();
            return false;
        }
        else
        {
            chess.undoMove();
            chess.generateBlackAttackZones();
            return true;
        }
    }

    inline bool isLegalBlackMove(std::string& move)
    {
        if (move.size() != 6)
            return false;

        int index = -1;

        for (int i = 0; i < chess.blackMoves.size(); i++)
        {
            if (chess.blackMoves[i] == move)
            {
                index = i;
                break;
            }
        }

        if (index == -1)
            return false;

        chess.doBlackMove(move);
        chess.generateWhiteAttackZones();

        if (chess.isBlackKingInCheck())
        {
            chess.undoMove();
            chess.generateWhiteAttackZones();
            return false;
        }
        else
        {
            chess.undoMove();
            chess.generateWhiteAttackZones();
            return true;
        }
    }

    inline bool areThereAnyLegalMovesForWhite()
    {
        for (int i = 0; i < chess.whiteMoves.size(); i++)
        {
            chess.doWhiteMove(chess.whiteMoves[i]);
            chess.generateBlackAttackZones();

            if (!chess.isWhiteKingInCheck())
            {
                chess.undoMove();
                chess.generateBlackAttackZones();
                return true;
            }

            chess.undoMove();
        }

        chess.generateBlackAttackZones();

        return false;
    }

    inline bool areThereAnyLegalMovesForBlack()
    {
        for (int i = 0; i < chess.blackMoves.size(); i++)
        {
            chess.doBlackMove(chess.blackMoves[i]);
            chess.generateWhiteAttackZones();

            if (!chess.isBlackKingInCheck())
            {
                chess.undoMove();
                chess.generateWhiteAttackZones();
                return true;
            }

            chess.undoMove();
        }

        chess.generateWhiteAttackZones();

        return false;
    }

    int evaluatePosition()
    {
        int score = 0;

        memset(this->pressureBoard, 0, sizeof(this->pressureBoard));

        for (int i = 0; i < chess.whiteMoves.size(); i++)
            this->pressureBoard[(chess.whiteMoves[i][3] - '0') * 8 + chess.whiteMoves[i][4] - '0']++;
        for (int i = 0; i < chess.blackMoves.size(); i++)
            this->pressureBoard[(chess.blackMoves[i][3] - '0') * 8 + chess.blackMoves[i][4] - '0']--;

        if (playerID == WHITE)
        {
            if (chess.whiteKingCastled)
            {
                score -= CASTLED_KING_SCORE;

                BitBoard pieces = chess.whiteKing;
                BitBoard piece = pieces & ~(pieces - 1);

                while (piece)
                {
                    int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                    if (pos / 8 == 0)
                    {
                        if (pos % 8 == 0 && ((chess.whitePawn & (1ll << 8)) == 0ll || (chess.whitePawn & (1ll << 9)) == 0ll))
                        {
                            score += STUPID_CASTLING;
                        }
                        else if (pos % 8 == 7 && ((chess.whitePawn & (1ll << 14)) == 0ll || (chess.whitePawn & (1ll << 15)) == 0ll))
                        {
                            score += STUPID_CASTLING;
                        }
                        else if ((chess.whitePawn & (1ll << (pos % 8 - 1))) == 0ll || (chess.whitePawn & (1ll << (pos % 8))) == 0ll || (chess.whitePawn & (1ll << (pos % 8 + 1))) == 0ll)
                        {
                            score += STUPID_CASTLING;
                        }
                    }

                    pieces &= ~piece;
                    piece = pieces & ~(pieces - 1);
                }
            }
            else if (!chess.whiteKingMoved && (!chess.whiteLeftRookMoved || !chess.whiteRightRookMoved))
                score -= CAN_CASTLE_KING_SCORE;
            if (chess.blackKingCastled)
            {
                score += CASTLED_KING_SCORE;

                BitBoard pieces = chess.blackKing;
                BitBoard piece = pieces & ~(pieces - 1);

                while (piece)
                {
                    int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                    if (pos / 8 == 7)
                    {
                        if (pos % 8 == 0 && ((chess.blackPawn & (1ll << 48)) == 0ll || (chess.blackPawn & (1ll << 49)) == 0ll))
                        {
                            score -= STUPID_CASTLING;
                        }
                        else if (pos % 8 == 7 && ((chess.blackPawn & (1ll << 54)) == 0ll || (chess.blackPawn & (1ll << 55)) == 0ll))
                        {
                            score -= STUPID_CASTLING;
                        }
                        else if ((chess.blackPawn & (1ll << (((pos / 8) - 1) * 8 + pos % 8 - 1))) == 0ll || (chess.blackPawn & (1ll << (((pos / 8) - 1) * 8 + pos % 8))) == 0ll || (chess.blackPawn & (1ll << (((pos / 8) - 1) * 8 + pos % 8 + 1))) == 0ll)
                        {
                            score -= STUPID_CASTLING;
                        }
                    }

                    pieces &= ~piece;
                    piece = pieces & ~(pieces - 1);
                }
            }
            else if (!chess.blackKingMoved && (!chess.blackLeftRookMoved || !chess.blackRightRookMoved))
                score += CAN_CASTLE_KING_SCORE;

            BitBoard pieces = chess.whitePawn;
            BitBoard piece = pieces & ~(pieces - 1);

            while (piece)
            {
                int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                score -= PAWN_SCORE;
                if (this->inEndGame)
                    score -= this->whitePawnPositionScore[pos] * 2;
                else
                    score -= this->whitePawnPositionScore[pos];
                ///score -= this->pressureBoard[pos] * this->PAWN_PRESSURE_SCORE;

                pieces &= ~piece;
                piece = pieces & ~(pieces - 1);
            }

            pieces = chess.whiteRook;
            piece = pieces & ~(pieces - 1);

            while (piece)
            {
                int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                score -= ROOK_SCORE;
                score -= this->whiteRookPositionScore[pos];
                score -= this->pressureBoard[pos] * this->ROOK_PRESSURE_SCORE;

                pieces &= ~piece;
                piece = pieces & ~(pieces - 1);
            }

            pieces = chess.whiteKnight;
            piece = pieces & ~(pieces - 1);

            while (piece)
            {
                int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                score -= KNIGHT_SCORE;
                score -= this->whiteKnightPositionScore[pos];
                score -= this->pressureBoard[pos] * this->KNIGHT_PRESSURE_SCORE;

                pieces &= ~piece;
                piece = pieces & ~(pieces - 1);
            }

            pieces = chess.whiteBishop;
            piece = pieces & ~(pieces - 1);

            while (piece)
            {
                int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                score -= BISHOP_SCORE;
                score -= this->whiteBishopPositionScore[pos];
                score -= this->pressureBoard[pos] * this->BISHOP_PRESSURE_SCORE;

                pieces &= ~piece;
                piece = pieces & ~(pieces - 1);
            }

            pieces = chess.whiteQueen;
            piece = pieces & ~(pieces - 1);

            while (piece)
            {
                int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                score -= QUEEN_SCORE;
                score -= this->whiteQueenPositionScore[pos];
                score -= this->pressureBoard[pos] * this->QUEEN_PRESSURE_SCORE;

                pieces &= ~piece;
                piece = pieces & ~(pieces - 1);
            }

            pieces = chess.whiteKing;
            piece = pieces & ~(pieces - 1);

            while (piece)
            {
                int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                score -= KING_SCORE;
                if (this->inEndGame)
                    score -= this->whiteKingPositionScore[pos] * (-1);
                else
                    score -= this->whiteKingPositionScore[pos];
                score -= this->pressureBoard[pos] * this->KING_PRESSURE_SCORE;

                pieces &= ~piece;
                piece = pieces & ~(pieces - 1);
            }

            pieces = chess.blackPawn;
            piece = pieces & ~(pieces - 1);

            while (piece)
            {
                int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                score += PAWN_SCORE;
                if (this->inEndGame)
                    score += this->blackPawnPositionScore[pos] * 10;
                else
                    score += this->blackPawnPositionScore[pos];
                ///score -= this->pressureBoard[pos] * this->PAWN_PRESSURE_SCORE;

                pieces &= ~piece;
                piece = pieces & ~(pieces - 1);
            }

            pieces = chess.blackRook;
            piece = pieces & ~(pieces - 1);

            while (piece)
            {
                int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                score += ROOK_SCORE;
                score += this->blackRookPositionScore[pos];
                score -= this->pressureBoard[pos] * this->ROOK_PRESSURE_SCORE;

                pieces &= ~piece;
                piece = pieces & ~(pieces - 1);
            }

            pieces = chess.blackKnight;
            piece = pieces & ~(pieces - 1);

            while (piece)
            {
                int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                score += KNIGHT_SCORE;
                score += this->blackKnightPositionScore[pos];
                score -= this->pressureBoard[pos] * this->KNIGHT_PRESSURE_SCORE;

                pieces &= ~piece;
                piece = pieces & ~(pieces - 1);
            }

            pieces = chess.blackBishop;
            piece = pieces & ~(pieces - 1);

            while (piece)
            {
                int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                score += BISHOP_SCORE;
                score += this->blackBishopPositionScore[pos];
                score -= this->pressureBoard[pos] * this->BISHOP_PRESSURE_SCORE;

                pieces &= ~piece;
                piece = pieces & ~(pieces - 1);
            }

            pieces = chess.blackQueen;
            piece = pieces & ~(pieces - 1);

            while (piece)
            {
                int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                score += QUEEN_SCORE;
                score += this->blackQueenPositionScore[pos];
                score -= this->pressureBoard[pos] * this->QUEEN_PRESSURE_SCORE;

                pieces &= ~piece;
                piece = pieces & ~(pieces - 1);
            }

            pieces = chess.blackKing;
            piece = pieces & ~(pieces - 1);

            while (piece)
            {
                int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                score += KING_SCORE;
                if (this->inEndGame)
                    score += this->blackKingPositionScore[pos] * (-1);
                else
                    score += this->blackKingPositionScore[pos];
                score -= this->pressureBoard[pos] * this->KING_PRESSURE_SCORE;

                pieces &= ~piece;
                piece = pieces & ~(pieces - 1);
            }
        }
        else
        {
            if (chess.whiteKingCastled)
            {
                score += CASTLED_KING_SCORE;

                BitBoard pieces = chess.whiteKing;
                BitBoard piece = pieces & ~(pieces - 1);

                while (piece)
                {
                    int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                    if (pos / 8 == 0)
                    {
                        if (pos % 8 == 0 && ((chess.whitePawn & (1ll << 8)) == 0ll || (chess.whitePawn & (1ll << 9)) == 0ll))
                        {
                            score -= STUPID_CASTLING;
                        }
                        else if (pos % 8 == 7 && ((chess.whitePawn & (1ll << 14)) == 0ll || (chess.whitePawn & (1ll << 15)) == 0ll))
                        {
                            score -= STUPID_CASTLING;
                        }
                        else if ((chess.whitePawn & (1ll << (pos % 8 - 1))) == 0ll || (chess.whitePawn & (1ll << (pos % 8))) == 0ll || (chess.whitePawn & (1ll << (pos % 8 + 1))) == 0ll)
                        {
                            score -= STUPID_CASTLING;
                        }
                    }

                    pieces &= ~piece;
                    piece = pieces & ~(pieces - 1);
                }
            }
            else if (!chess.whiteKingMoved && (!chess.whiteLeftRookMoved || !chess.whiteRightRookMoved))
                score += CAN_CASTLE_KING_SCORE;
            if (chess.blackKingCastled)
            {
                score -= CASTLED_KING_SCORE;

                BitBoard pieces = chess.blackKing;
                BitBoard piece = pieces & ~(pieces - 1);

                while (piece)
                {
                    int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                    if (pos / 8 == 7)
                    {
                        if (pos % 8 == 0 && ((chess.blackPawn & (1ll << 48)) == 0ll || (chess.blackPawn & (1ll << 49)) == 0ll))
                        {
                            score += STUPID_CASTLING;
                        }
                        else if (pos % 8 == 7 && ((chess.blackPawn & (1ll << 54)) == 0ll || (chess.blackPawn & (1ll << 55)) == 0ll))
                        {
                            score += STUPID_CASTLING;
                        }
                        else if ((chess.blackPawn & (1ll << (((pos / 8) - 1) * 8 + pos % 8 - 1))) == 0ll || (chess.blackPawn & (1ll << (((pos / 8) - 1) * 8 + pos % 8))) == 0ll || (chess.blackPawn & (1ll << (((pos / 8) - 1) * 8 + pos % 8 + 1))) == 0ll)
                        {
                            score += STUPID_CASTLING;
                        }
                    }

                    pieces &= ~piece;
                    piece = pieces & ~(pieces - 1);
                }
            }
            else if (!chess.blackKingMoved && (!chess.blackLeftRookMoved || !chess.blackRightRookMoved))
                score -= CAN_CASTLE_KING_SCORE;

            BitBoard pieces = chess.whitePawn;
            BitBoard piece = pieces & ~(pieces - 1);

            while (piece)
            {
                int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                score += PAWN_SCORE;
                if (this->inEndGame)
                    score += this->whitePawnPositionScore[pos] * 2;
                else
                    score += this->whitePawnPositionScore[pos];
                ///score += this->pressureBoard[pos] * this->PAWN_PRESSURE_SCORE;

                pieces &= ~piece;
                piece = pieces & ~(pieces - 1);
            }

            pieces = chess.whiteRook;
            piece = pieces & ~(pieces - 1);

            while (piece)
            {
                int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                score += ROOK_SCORE;
                score += this->whiteRookPositionScore[pos];
                score += this->pressureBoard[pos] * this->ROOK_PRESSURE_SCORE;

                pieces &= ~piece;
                piece = pieces & ~(pieces - 1);
            }

            pieces = chess.whiteKnight;
            piece = pieces & ~(pieces - 1);

            while (piece)
            {
                int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                score += KNIGHT_SCORE;
                score += this->whiteKnightPositionScore[pos];
                score += this->pressureBoard[pos] * this->KNIGHT_PRESSURE_SCORE;

                pieces &= ~piece;
                piece = pieces & ~(pieces - 1);
            }

            pieces = chess.whiteBishop;
            piece = pieces & ~(pieces - 1);

            while (piece)
            {
                int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                score += BISHOP_SCORE;
                score += this->whiteBishopPositionScore[pos];
                score += this->pressureBoard[pos] * this->BISHOP_PRESSURE_SCORE;

                pieces &= ~piece;
                piece = pieces & ~(pieces - 1);
            }

            pieces = chess.whiteQueen;
            piece = pieces & ~(pieces - 1);

            while (piece)
            {
                int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                score += QUEEN_SCORE;
                score += this->whiteQueenPositionScore[pos];
                score += this->pressureBoard[pos] * this->QUEEN_PRESSURE_SCORE;

                pieces &= ~piece;
                piece = pieces & ~(pieces - 1);
            }

            pieces = chess.whiteKing;
            piece = pieces & ~(pieces - 1);

            while (piece)
            {
                int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                score += KING_SCORE;
                if (this->inEndGame)
                    score += this->whiteKingPositionScore[pos] * (-1);
                else
                    score += this->whiteKingPositionScore[pos];
                score += this->pressureBoard[pos] * this->KING_PRESSURE_SCORE;

                pieces &= ~piece;
                piece = pieces & ~(pieces - 1);
            }

            pieces = chess.blackPawn;
            piece = pieces & ~(pieces - 1);

            while (piece)
            {
                int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                score -= PAWN_SCORE;
                if (this->inEndGame)
                    score -= this->blackPawnPositionScore[pos] * 10;
                else
                    score -= this->blackPawnPositionScore[pos];
                ///score += this->pressureBoard[pos] * this->PAWN_PRESSURE_SCORE;

                pieces &= ~piece;
                piece = pieces & ~(pieces - 1);
            }

            pieces = chess.blackRook;
            piece = pieces & ~(pieces - 1);

            while (piece)
            {
                int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                score -= ROOK_SCORE;
                score -= this->blackRookPositionScore[pos];
                score += this->pressureBoard[pos] * this->ROOK_PRESSURE_SCORE;

                pieces &= ~piece;
                piece = pieces & ~(pieces - 1);
            }

            pieces = chess.blackKnight;
            piece = pieces & ~(pieces - 1);

            while (piece)
            {
                int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                score -= KNIGHT_SCORE;
                score -= this->blackKnightPositionScore[pos];
                score += this->pressureBoard[pos] * this->KNIGHT_PRESSURE_SCORE;

                pieces &= ~piece;
                piece = pieces & ~(pieces - 1);
            }

            pieces = chess.blackBishop;
            piece = pieces & ~(pieces - 1);

            while (piece)
            {
                int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                score -= BISHOP_SCORE;
                score -= this->blackBishopPositionScore[pos];
                score += this->pressureBoard[pos] * this->BISHOP_PRESSURE_SCORE;

                pieces &= ~piece;
                piece = pieces & ~(pieces - 1);
            }

            pieces = chess.blackQueen;
            piece = pieces & ~(pieces - 1);

            while (piece)
            {
                int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                score -= QUEEN_SCORE;
                score -= this->blackQueenPositionScore[pos];
                score += this->pressureBoard[pos] * this->QUEEN_PRESSURE_SCORE;

                pieces &= ~piece;
                piece = pieces & ~(pieces - 1);
            }

            pieces = chess.blackKing;
            piece = pieces & ~(pieces - 1);

            while (piece)
            {
                int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

                score -= KING_SCORE;
                if (this->inEndGame)
                    score -= this->blackKingPositionScore[pos] * (-1);
                else
                    score -= this->blackKingPositionScore[pos];
                score += this->pressureBoard[pos] * this->KING_PRESSURE_SCORE;

                pieces &= ~piece;
                piece = pieces & ~(pieces - 1);
            }
        }

        return score;
    }

    int evaluateMove(int ID, std::string& move)
    {
        int score = 0;

        if (ID == WHITE)
        {
            if (chess.occupied & (1ll << ((move[3] - '0') * 8 + move[4] - '0')))
            {
                if (move[0] == 'P')
                    score += 60;
                else if (move[0] == 'N')
                    score += 50;
                else if (move[0] == 'B')
                    score += 40;
                else if (move[0] == 'R')
                    score += 30;
                else if (move[0] == 'Q')
                    score += 20;
                else
                    score += 10;

                if (chess.blackPawn & (1ll << ((move[3] - '0') * 8 + move[4] - '0')))
                    score += 1;
                else if (chess.blackKnight & (1ll << ((move[3] - '0') * 8 + move[4] - '0')))
                    score += 2;
                else if (chess.blackBishop & (1ll << ((move[3] - '0') * 8 + move[4] - '0')))
                    score += 3;
                else if (chess.blackRook & (1ll << ((move[3] - '0') * 8 + move[4] - '0')))
                    score += 4;
                else if (chess.blackQueen & (1ll << ((move[3] - '0') * 8 + move[4] - '0')))
                    score += 5;
            }
        }
        else
        {
            if (chess.occupied & (1ll << ((move[3] - '0') * 8 + move[4] - '0')))
            {
                if (move[0] == 'p')
                    score += 60;
                else if (move[0] == 'n')
                    score += 50;
                else if (move[0] == 'b')
                    score += 40;
                else if (move[0] == 'r')
                    score += 30;
                else if (move[0] == 'q')
                    score += 20;
                else
                    score += 10;

                if (chess.whitePawn & (1ll << ((move[3] - '0') * 8 + move[4] - '0')))
                    score += 1;
                else if (chess.whiteKnight & (1ll << ((move[3] - '0') * 8 + move[4] - '0')))
                    score += 2;
                else if (chess.whiteBishop & (1ll << ((move[3] - '0') * 8 + move[4] - '0')))
                    score += 3;
                else if (chess.whiteRook & (1ll << ((move[3] - '0') * 8 + move[4] - '0')))
                    score += 4;
                else if (chess.whiteQueen & (1ll << ((move[3] - '0') * 8 + move[4] - '0')))
                    score += 5;
            }
        }

        return score;
    }

    void positionString(std::string& positionFEN, int currentID)
    {
        positionFEN.clear();
        positionFEN.resize(70, '$');

        BitBoard pieces = chess.whitePawn;
        BitBoard piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

            positionFEN[pos] = 'P';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        pieces = chess.whiteRook;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

            positionFEN[pos] = 'R';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        pieces = chess.whiteKnight;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

            positionFEN[pos] = 'N';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        pieces = chess.whiteBishop;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

            positionFEN[pos] = 'B';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        pieces = chess.whiteQueen;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

            positionFEN[pos] = 'Q';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        pieces = chess.whiteKing;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

            positionFEN[pos] = 'K';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        pieces = chess.blackPawn;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

            positionFEN[pos] = 'p';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        pieces = chess.blackRook;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

            positionFEN[pos] = 'r';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        pieces = chess.blackKnight;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

            positionFEN[pos] = 'n';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        pieces = chess.blackBishop;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

            positionFEN[pos] = 'b';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        pieces = chess.blackQueen;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

            positionFEN[pos] = 'q';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        pieces = chess.blackKing;
        piece = pieces & ~(pieces - 1);

        while (piece)
        {
            int pos = chess.log2[piece % chess.LOG2_MODULO + chess.LOG2_MODULO];

            positionFEN[pos] = 'k';

            pieces &= ~piece;
            piece = pieces & ~(pieces - 1);
        }

        if (!chess.isWhiteKingInCheck() && !chess.whiteKingMoved)
        {
            if (!chess.whiteLeftRookMoved && !((chess.whiteLeftSide & chess.occupied) || (chess.whiteLeftSide & chess.blackPieceAttackZones)))
                positionFEN[64] = 'Y';
            else
                positionFEN[64] = 'N';
            if (!chess.whiteRightRookMoved && !((chess.whiteRightSide & chess.blackPieceAttackZones) | (chess.whiteExtendedRightSide & chess.occupied)))
                positionFEN[65] = 'Y';
            else
                positionFEN[65] = 'N';
        }

        if (!chess.isBlackKingInCheck() && !chess.blackKingMoved)
        {
            if (!chess.blackLeftRookMoved && !((chess.blackLeftSide & chess.occupied) || (chess.blackLeftSide & chess.whitePieceAttackZones)))
                positionFEN[66] = 'Y';
            else
                positionFEN[66] = 'N';
            if (!chess.blackRightRookMoved && !((chess.blackRightSide & chess.whitePieceAttackZones) | (chess.blackExtendedRightSide & chess.occupied)))
                positionFEN[67] = 'Y';
            else
                positionFEN[67] = 'N';
        }

        if (chess.enPassant != 0ll)
            positionFEN[68] = chess.log2[chess.enPassant % chess.LOG2_MODULO + chess.LOG2_MODULO] % 8 + '0';

        positionFEN[69] = currentID + '0';
    }

    std::pair<int, std::string> searchMoves(bool playerTurn, int currentID, int min, int max, int depth)
    {
        this->numNodes++;

        std::string positionFEN = "";

        positionString(positionFEN, currentID);

        chess.positionFreq[positionFEN]++;

        std::string positionFENDepth = positionFEN;
        positionFENDepth += depth + '0';

        if (chess.positionFreq[positionFEN] == 3)
        {
            chess.positionFreq[positionFEN]--;
            chess.positionScore[positionFENDepth] = std::make_pair(0, "");
            return std::make_pair(0, "");
        }

        if (chess.positionScore.find(positionFENDepth) != chess.positionScore.end())
        {
            chess.positionFreq[positionFEN]--;
            return chess.positionScore[positionFENDepth];
        }

        if (currentID == WHITE)
        {
            chess.generateBlackMoves();
            chess.generateWhiteMoves();
        }
        else
        {
            chess.generateWhiteMoves();
            chess.generateBlackMoves();
        }

        int gameStatus = gameOver(currentID, positionFEN);

        if (gameStatus != GAME_CONTINUES)
        {
            if (gameStatus == DRAW)
            {
                chess.positionFreq[positionFEN]--;
                chess.positionScore[positionFENDepth] = std::make_pair(0, "");
                return std::make_pair(0, "");
            }
            else
            {
                if (playerTurn)
                {
                    chess.positionFreq[positionFEN]--;
                    chess.positionScore[positionFENDepth] = std::make_pair(+REACHABLE_INF, "");
                    return std::make_pair(+REACHABLE_INF + depth, "");
                }
                else
                {
                    chess.positionFreq[positionFEN]--;
                    chess.positionScore[positionFENDepth] = std::make_pair(-REACHABLE_INF, "");
                    return std::make_pair(-REACHABLE_INF - depth, "");
                }
            }
        }

        if (depth == 0)
        {
            int positionScore = evaluatePosition();

            chess.positionFreq[positionFEN]--;
            chess.positionScore[positionFENDepth] = std::make_pair(positionScore, "");
            return std::make_pair(positionScore, "");
        }

        if (playerTurn)
        {
            std::string bestMove = "";
            int bestScore = INF;

            if (currentID == WHITE)
            {
                std::vector<std::pair<int, std::string>> moves;

                for (int i = 0; i < chess.whiteMoves.size(); i++)
                    moves.emplace_back(make_pair(evaluateMove(WHITE, chess.whiteMoves[i]), chess.whiteMoves[i]));

                std::sort(moves.begin(), moves.end(), compMoves);

                for (int i = 0; i < moves.size(); i++)
                {
                    chess.doWhiteMove(moves[i].second);
                    chess.generateBlackAttackZones();

                    if (!chess.isWhiteKingInCheck())
                    {
                        int score = searchMoves(!playerTurn, 1 - currentID, min, max, depth - 1).first;

                        if (score < bestScore)
                        {
                            bestScore = score;
                            bestMove = moves[i].second;

                            min = bestScore;
                        }

                        if (min <= max)
                        {
                            chess.undoMove();
                            break;
                        }
                    }

                    chess.undoMove();
                }
            }
            else
            {
                std::vector<std::pair<int, std::string>> moves;

                for (int i = 0; i < chess.blackMoves.size(); i++)
                    moves.emplace_back(make_pair(evaluateMove(BLACK, chess.blackMoves[i]), chess.blackMoves[i]));

                std::sort(moves.begin(), moves.end(), compMoves);

                for (int i = 0; i < moves.size(); i++)
                {
                    chess.doBlackMove(moves[i].second);
                    chess.generateWhiteAttackZones();

                    if (!chess.isBlackKingInCheck())
                    {
                        int score = searchMoves(!playerTurn, 1 - currentID, min, max, depth - 1).first;

                        if (score < bestScore)
                        {
                            bestScore = score;
                            bestMove = moves[i].second;

                            min = bestScore;
                        }

                        if (min <= max)
                        {
                            chess.undoMove();
                            break;
                        }
                    }

                    chess.undoMove();
                }
            }

            chess.positionFreq[positionFEN]--;
            chess.positionScore[positionFENDepth] = make_pair(bestScore, bestMove);
            return std::make_pair(bestScore, bestMove);
        }
        else
        {
            std::string bestMove = "";
            int bestScore = -INF;

            if (currentID == WHITE)
            {
                std::vector<std::pair<int, std::string>> moves;

                for (int i = 0; i < chess.whiteMoves.size(); i++)
                    moves.emplace_back(make_pair(evaluateMove(WHITE, chess.whiteMoves[i]), chess.whiteMoves[i]));

                std::sort(moves.begin(), moves.end(), compMoves);

                for (int i = 0; i < moves.size(); i++)
                {
                    chess.doWhiteMove(moves[i].second);
                    chess.generateBlackAttackZones();

                    if (!chess.isWhiteKingInCheck())
                    {
                        int score = searchMoves(!playerTurn, 1 - currentID, min, max, depth - 1).first;

                        if (score > bestScore)
                        {
                            bestScore = score;
                            bestMove = moves[i].second;

                            max = bestScore;
                        }

                        if (min <= max)
                        {
                            chess.undoMove();
                            break;
                        }
                    }

                    chess.undoMove();
                }
            }
            else
            {
                std::vector<std::pair<int, std::string>> moves;

                for (int i = 0; i < chess.blackMoves.size(); i++)
                    moves.emplace_back(make_pair(evaluateMove(BLACK, chess.blackMoves[i]), chess.blackMoves[i]));

                std::sort(moves.begin(), moves.end(), compMoves);

                for (int i = 0; i < moves.size(); i++)
                {
                    chess.doBlackMove(moves[i].second);
                    chess.generateWhiteAttackZones();

                    if (!chess.isBlackKingInCheck())
                    {
                        int score = searchMoves(!playerTurn, 1 - currentID, min, max, depth - 1).first;

                        if (score > bestScore)
                        {
                            bestScore = score;
                            bestMove = moves[i].second;

                            max = bestScore;
                        }

                        if (min <= max)
                        {
                            chess.undoMove();
                            break;
                        }
                    }

                    chess.undoMove();
                }
            }

            chess.positionFreq[positionFEN]--;
            chess.positionScore[positionFENDepth] = make_pair(bestScore, bestMove);
            return std::make_pair(bestScore, bestMove);
        }
    }

    int gameOver(int ID, std::string& positionFEN)
    {
        if (chess.positionFreq[positionFEN] == 3)
            return DRAW;

        if (ID == WHITE)
        {
            if (!areThereAnyLegalMovesForWhite())
            {
                if (chess.isWhiteKingInCheck())
                    return BLACK_WON;
                else
                    return DRAW;
            }
        }
        else
        {
            if (!areThereAnyLegalMovesForBlack())
            {
                if (chess.isBlackKingInCheck())
                    return WHITE_WON;
                else
                    return DRAW;
            }
        }

        return GAME_CONTINUES;
    }

    void run()
    {
        int currentID = WHITE;
        bool playerTurn = this->playerID == 1 ? WHITE : BLACK;

        announceNextPlayer(currentID);

        chess.refreshBoard();
        chess.outputBoard(this->displayMode);

        chess.generateBlackAttackZones();
        chess.generateWhiteMoves();

        std::string positionFEN;
        positionString(positionFEN, currentID);
        chess.positionFreq[positionFEN]++;

        announceCheck(currentID);

        int gameStatus = gameOver(currentID, positionFEN);

        std::string lastMove = "";

        while (gameStatus == GAME_CONTINUES)
        {
            if (playerTurn)
            {
                std::string move;
                std::cin >> move;
                transformMove(move);

                while ((currentID == WHITE && !isLegalWhiteMove(move)) || (currentID == BLACK && !isLegalBlackMove(move)))
                {
                    std::cout << "You can not do that! Try another move." << '\n';
                    std::cin >> move;
                    transformMove(move);
                }

                if (currentID == this->WHITE)
                    chess.doWhiteMove(move);
                else
                    chess.doBlackMove(move);

                lastMove = move;

                /*
                this->numNodes = 0;
                chess.positionScore.clear();
                chess.positionFreq[positionFEN]--;
                std::string move = searchMoves(playerTurn, currentID, +INF, -INF, this->MAX_SEARCH_DEPTH).second;
                chess.positionFreq[positionFEN]++;

                if (currentID == this->WHITE)
                    chess.doWhiteMove(move);
                else
                    chess.doBlackMove(move);

                lastMove = move;

                if (this->numNodes <= 2000)
                    this->MAX_SEARCH_DEPTH++;
                if (this->numNodes >= 90000)
                    this->MAX_SEARCH_DEPTH--;
                */
            }
            else
            {
                this->numNodes = 0;
                chess.positionScore.clear();
                chess.positionFreq[positionFEN]--;
                std::string move = searchMoves(playerTurn, currentID, +INF, -INF, this->MAX_SEARCH_DEPTH).second;
                chess.positionFreq[positionFEN]++;

                if (currentID == this->WHITE)
                    chess.doWhiteMove(move);
                else
                    chess.doBlackMove(move);

                lastMove = move;

                if (this->numNodes <= 2000)
                    this->MAX_SEARCH_DEPTH++;
                if (this->numNodes >= 90000)
                    this->MAX_SEARCH_DEPTH--;

                /*
                std::string move;
                std::cin >> move;
                transformMove(move);

                while ((currentID == WHITE && !isLegalWhiteMove(move)) || (currentID == BLACK && !isLegalBlackMove(move)))
                {
                    std::cout << "You can not do that! Try another move." << '\n';
                    std::cin >> move;
                    transformMove(move);
                }

                if (currentID == this->WHITE)
                    chess.doWhiteMove(move);
                else
                    chess.doBlackMove(move);

                lastMove = move;
                */
            }

            system("CLS");

            currentID = 1 - currentID;
            playerTurn = !playerTurn;

            if (currentID == WHITE)
            {
                chess.generateBlackAttackZones();
                chess.generateWhiteMoves();
            }
            else
            {
                chess.generateWhiteAttackZones();
                chess.generateBlackMoves();
            }

            announceNextPlayer(currentID);

            chess.refreshBoard();
            chess.outputBoard(this->displayMode);

            std::cout << "Last move:" << '\n';
            std::cout << lastMove << '\n';

            std::cout << "Number of nodes searched: " << '\n';
            std::cout << this->numNodes << '\n';

            std::cout << "Current search depth: " << '\n';
            std::cout << this->MAX_SEARCH_DEPTH << '\n';

            this->inEndGame = isInEndGame();

            positionString(positionFEN, currentID);
            chess.positionFreq[positionFEN]++;

            gameStatus = gameOver(currentID, positionFEN);

            if (gameStatus == GAME_CONTINUES)
                announceCheck(currentID);
        }

        if (gameStatus == DRAW)
            std::cout << "It is a draw!" << '\n';
        else if (gameStatus == WHITE_WON)
            std::cout << "Checkmate! White won!" << '\n';
        else
            std::cout << "Checkmate! Black won!" << '\n';
    }
};

Game game;

int main()
{
    chess.initializeBoard();

    game.initialize();

    game.run();

    char junk;
    std::cin >> junk;

    return 0;
}
