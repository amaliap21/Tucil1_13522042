#include "helpers.h"
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
        return; // Ensure we don't exceed buffer size and avoid revisiting tokens

    visited.push_back(matrix[x][y]); // Mark current token as visited

    // Add current token to buffer
    buffer.push_back(matrix[x][y]);

    // Check each sequence
    for (int i = 0; i < sequences.size(); ++i)
    {
        if (!sequenceMatched[i] && isSequenceMatched(sequences[i], currentReward, visited))
        {
            sequenceMatched[i] = true; // Mark sequence as matched
        }
    }

    // Alternate between vertical and horizontal movements
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

    // Remove the last token (backtrack) and remove current token from visited list
    buffer.pop_back();
    visited.pop_back();
    for (int i = 0; i < sequences.size(); ++i)
    {
        sequenceMatched[i] = false; // Reset sequence matched status
    }
}

void BreachProtocolSolver::solve()
{
    vector<bool> sequenceMatched(sequences.size(), false);
    vector<Token> visited; // Initialize visited vector
    // Initial call to DFS for each token in the first row
    for (int y = 0; y < matrix[0].size(); ++y)
    {
        dfs(0, y, 0, 0, sequenceMatched, visited); // Start with horizontal movement
    }
}

bool BreachProtocolSolver::isSequenceMatched(const Sequence &sequence, int &currentReward, const vector<Token> &visited)
{
    // Check if sequence is matched in the buffer
    if (buffer.size() < sequence.tokens.size())
        return false; // Buffer is too small

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
        currentReward += sequence.reward; // Update current reward
        if (currentReward > maxReward)
        {
            maxReward = currentReward; // Update max reward
            solutionBuffer = buffer;   // Update solution buffer
        }
        return true;
    }

    return false;
}

