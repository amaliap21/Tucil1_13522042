#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <vector>
#include <string>

using namespace std;

struct Token
{
    string value;
    int x, y;
    Token(string val, int xCoord, int yCoord) : value(val), x(xCoord), y(yCoord) {}
};

struct Sequence
{
    vector<string> tokens;
    int reward;
    Sequence(vector<string> tok, int rew) : tokens(tok), reward(rew) {}
};

class BreachProtocolSolver
{
public:
    BreachProtocolSolver(int bufferSize, const vector<vector<string>> &matrix, const vector<Sequence> &sequences);
    void solve();
    void printSolution();
    bool hasBeenVisited(vector<Token> &visited, Token &token);

private:
    int bufferSize;
    vector<vector<Token>> matrix;
    vector<Sequence> sequences;
    vector<Token> buffer;
    int maxReward;
    vector<Token> solutionBuffer;
    void dfs(int x, int y, int direction, int currentReward, vector<bool> &sequenceMatched, vector<Token> &visited);
    bool isSequenceMatched(const Sequence &sequence, int &currentReward, const vector<Token> &visited);
};

#endif
