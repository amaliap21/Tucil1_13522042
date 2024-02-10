#include "solver.hpp"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

BreachProtocolSolver::BreachProtocolSolver(int bufferSize, const vector<vector<string>> &matrixInput, const vector<Sequence> &sequencesInput)
    : bufferSize(bufferSize), sequences(sequencesInput), maxReward(0)
{
    // Konversi matrix input menjadi vector of vector of Token
    for (int i = 0; i < matrixInput.size(); i++)
    {
        vector<Token> row;
        for (int j = 0; j < matrixInput[i].size(); j++)
        {
            row.emplace_back(matrixInput[i][j], i, j);
        }
        matrix.push_back(row);
    }
}

bool BreachProtocolSolver::hasBeenVisited(vector<Token> &visited, Token &token)
{
    for (auto &visitedToken : visited)
    {
        if (visitedToken.x == token.x && visitedToken.y == token.y)
        {
            return true;
        }
    }
    return false;
}

void BreachProtocolSolver::dfs(int x, int y, int direction, int currentReward, vector<bool> &sequenceMatched, vector<Token> &visited)
{
    if (buffer.size() > bufferSize || hasBeenVisited(visited, matrix[x][y]))
        return;

    visited.push_back(matrix[x][y]); // Mark current token as visited

    // Tambahkan token ke buffer
    buffer.push_back(matrix[x][y]);

    // Cek apakah buffer cocok dengan sequence
    for (int i = 0; i < sequences.size(); ++i)
    {
        if (!sequenceMatched[i] && isSequenceMatched(sequences[i], currentReward, visited))
        {
            sequenceMatched[i] = true;
        }
    }

    // Pergerakan Rekursif (DFS) Vertikal atau Horizontal
    if (direction == 0)
    {
        // Vertical
        for (int newX = 0; newX < matrix.size(); ++newX)
        {
            if (newX != x)
            {
                dfs(newX, y, 1, currentReward, sequenceMatched, visited);
            }
        }
    }
    else
    {
        // Horizontal
        for (int newY = 0; newY < matrix[0].size(); ++newY)
        {
            if (newY != y)
            {
                dfs(x, newY, 0, currentReward, sequenceMatched, visited);
            }
        }
    }

    // Backtracking
    buffer.pop_back();
    visited.pop_back();
    for (int i = 0; i < sequences.size(); ++i)
    {
        sequenceMatched[i] = false; // Reset sequenceMatched
    }
}

void BreachProtocolSolver::solve()
{
    vector<bool> sequenceMatched(sequences.size(), false);
    vector<Token> visited;

    // Berjalan ke setiap token di baris pertama
    for (int y = 0; y < matrix[0].size(); ++y)
    {
        dfs(0, y, 0, 0, sequenceMatched, visited);
    }
}

bool BreachProtocolSolver::isSequenceMatched(const Sequence &sequence, int &currentReward, const vector<Token> &visited)
{
    // Cek apakah buffer cocok dengan sequence
    if (buffer.size() < sequence.tokens.size())
        return false;

    bool matched = true;
    for (int i = 0; i < sequence.tokens.size(); ++i)
    {
        if (sequence.tokens[i] != buffer[buffer.size() - sequence.tokens.size() + i].value)
        {
            matched = false;
            break;
        }
    }

    if (matched)
    {
        currentReward += sequence.reward;
        // Max reward yang didapatkan
        if (currentReward > maxReward)
        {
            maxReward = currentReward;
            solutionBuffer = buffer;
        }
        return true;
    }

    return false;
}

void BreachProtocolSolver::printSolution()
{
    if (!solutionBuffer.empty())
    {
        cout << maxReward << endl;
        for (const auto &token : solutionBuffer)
        {
            cout << token.value << " ";
        }
        cout << endl;
        for (const auto &token : solutionBuffer)
        {
            cout << token.y + 1 << ", " << token.x + 1 << endl;
        }
    }

    else
    {
        cout << "Tidak ada buffer dan sequence yang cocok. Tidak ada solusi yang ditemukan!" << endl;
        return;
    }
}