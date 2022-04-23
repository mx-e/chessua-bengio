#include <vector>
#include <string>
#include <iostream>
#include "utils.hpp"

std::vector<std::string> split_string(std::string str, std::string delimiter)
{
    std::vector<std::string> str_list;
    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos)
    {
        token = str.substr(0, pos);
        str_list.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    str_list.push_back(str);
    return str_list;
}

int fen_char_to_board_state(char const symbol)
{
    switch (symbol)
    {
    case 'r':
        return black_rook;
    case 'R':
        return white_rook;
    case 'n':
        return black_knight;
    case 'N':
        return white_knight;
    case 'b':
        return black_bishop;
    case 'B':
        return white_bishop;
    case 'q':
        return black_queen;
    case 'Q':
        return white_queen;
    case 'k':
        return black_king;
    case 'K':
        return white_king;
    case 'p':
        return black_pawn;
    case 'P':
        return white_pawn;
    default:
        return -1;
    }
}

int fen_castling_char_to_idx(char const symbol)
{
    switch (symbol)
    {
    case 'q':
        return black_queenside;
    case 'Q':
        return white_queenside;
    case 'k':
        return black_kingside;
    case 'K':
        return white_kingside;
    default:
        return -1;
    }
}

int char_to_int(char const ch)
{
    int out = ch - '0';
    return out;
}

int abc_to_123(char const col_char)
{
    int char_int = col_char;
    if (char_int < 97 || char_int > 122)
    {
        return -1;
    }
    return char_int - 96;
}

char board_state_to_char(int state)
{
    switch (state)
    {
    case black_pawn:
        return 'p';
    case white_pawn:
        return 'P';
    case black_rook:
        return 'r';
    case white_rook:
        return 'R';
    case black_knight:
        return 'n';
    case white_knight:
        return 'N';
    case black_bishop:
        return 'b';
    case white_bishop:
        return 'B';
    case black_queen:
        return 'q';
    case white_queen:
        return 'Q';
    default:
        return ' ';
    }
}