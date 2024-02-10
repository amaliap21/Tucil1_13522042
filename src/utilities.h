#ifndef HELPERS_H
#define HELPERS_H

#include <vector>
#include <string>

// Definisi struktur data Token
struct Token
{
    std::string identifier;
    struct
    {
        int baris;
        int kolom;
    } position;
};

// Definisi struktur data Sequence
struct Sequence
{
    std::vector<Token> tokens;
    int length;
    int prize;
};

// Definisi struktur data Matrix
struct Matrix
{
    int row;
    int col;
    std::vector<std::vector<Token>> element;
};

#endif /* HELPERS_H */
