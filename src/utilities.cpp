#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;

struct Point
{
    int row;
    int col;
};

struct Token
{
    string identifier;
    Point position;
};

struct Sequence
{
    vector<Token> tokens;
    int length;
    int prize;
};

struct Matrix
{
    vector<vector<Token>> elements;
    int rows;
    int cols;
};

vector<Sequence> prizeSequences;
Sequence maxSequence;
Matrix evaluateMatrix;
int actualPrizeMax = 0;
int currPrizeMax = 0;

// Function to check if a point is within matrix bounds
bool isValid(Point p, int rows, int cols)
{
    return p.row >= 0 && p.row < rows && p.col >= 0 && p.col < cols;
}

bool operator<(const Token &t1, const Token &t2)
{
    // Bandingkan identifiernya
    return t1.identifier < t2.identifier;
}

// Depth-First Search (DFS) function to generate paths and evaluate prizes
void dfs(Point current, int remainingSteps, Sequence &currentSequence, vector<vector<bool>> &visited)
{
    if (remainingSteps == 0)
    {
        int prize = 0;
        for (const auto &sequence : prizeSequences)
        {
            if (includes(currentSequence.tokens.begin(), currentSequence.tokens.end(),
                         sequence.tokens.begin(), sequence.tokens.end()))
            {
                prize += sequence.prize;
            }
        }
        if (prize > currPrizeMax)
        {
            currPrizeMax = prize;
            maxSequence = currentSequence;
        }
    }
    else
    {
        const vector<Point> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Up, Down, Left, Right
        for (const auto &dir : directions)
        {
            Point next = {current.row + dir.row, current.col + dir.col};
            if (isValid(next, evaluateMatrix.rows, evaluateMatrix.cols) && !visited[next.row][next.col])
            {
                visited[next.row][next.col] = true;
                currentSequence.tokens.push_back(evaluateMatrix.elements[next.row][next.col]);
                dfs(next, remainingSteps - 1, currentSequence, visited);
                currentSequence.tokens.pop_back();
                visited[next.row][next.col] = false;
            }
        }
    }
}

// Function to print the result
void printResult(int diff)
{
    if (currPrizeMax > 0)
    {
        cout << "Maximum reward: " << currPrizeMax << endl;
        cout << "Sequence: ";
        for (const auto &token : maxSequence.tokens)
        {
            cout << token.identifier << " ";
        }
        cout << endl
             << "Paths: " << endl;
        for (const auto &token : maxSequence.tokens)
        {
            cout << "(" << token.position.row + 1 << "," << token.position.col + 1 << ")" << endl;
        }
    }
    else
    {
        cout << "There is no optimal solution for this problem :(" << endl;
    }
    cout << "Time: " << diff << " ms" << endl;
}

// Main function
int main()
{
    string filename;
    cout << "Enter the filename to read the matrix and sequences (relative to the test/input folder): ";
    cin >> filename;
    filename = "../test/" + filename;

    ifstream inputFile(filename);
    if (!inputFile.is_open())
    {
        cerr << "Unable to open file " << filename << endl;
        return 1;
    }

    int bufferSize;
    inputFile >> bufferSize;
    evaluateMatrix.rows = 0;
    inputFile >> evaluateMatrix.rows >> evaluateMatrix.cols;
    evaluateMatrix.elements.resize(evaluateMatrix.rows, vector<Token>(evaluateMatrix.cols));

    // Read matrix elements
    for (int i = 0; i < evaluateMatrix.rows; ++i)
    {
        for (int j = 0; j < evaluateMatrix.cols; ++j)
        {
            string identifier;
            inputFile >> identifier;
            evaluateMatrix.elements[i][j] = {identifier, {i, j}};
        }
    }

    // Read sequences
    int numSequences;
    inputFile >> numSequences;
    for (int i = 0; i < numSequences; ++i)
    {
        int length;
        inputFile >> length;
        Sequence sequence;
        sequence.length = length;
        for (int j = 0; j < length; ++j)
        {
            string identifier;
            inputFile >> identifier;
            for (int r = 0; r < evaluateMatrix.rows; ++r)
            {
                for (int c = 0; c < evaluateMatrix.cols; ++c)
                {
                    if (evaluateMatrix.elements[r][c].identifier == identifier)
                    {
                        sequence.tokens.push_back(evaluateMatrix.elements[r][c]);
                    }
                }
            }
        }
        inputFile >> sequence.prize;
        actualPrizeMax += sequence.prize;
        prizeSequences.push_back(sequence);
    }
    inputFile.close();

    auto start = chrono::steady_clock::now();

    vector<vector<bool>> visited(evaluateMatrix.rows, vector<bool>(evaluateMatrix.cols, false));
    for (int i = 0; i < evaluateMatrix.rows; ++i)
    {
        for (int j = 0; j < evaluateMatrix.cols; ++j)
        {
            visited[i][j] = true;
            Sequence startSequence = {{evaluateMatrix.elements[i][j]}, 1, 0};
            dfs({i, j}, bufferSize - 1, startSequence, visited);
            visited[i][j] = false;
        }
    }

    auto end = chrono::steady_clock::now();
    auto diff = chrono::duration_cast<chrono::milliseconds>(end - start);

    printResult(diff.count());

    char saveSolution;
    cout << "Do you want to save the solution? (y/n): ";
    cin >> saveSolution;
    if (saveSolution == 'y' || saveSolution == 'Y')
    {
        string outputFilename;
        cout << "Enter the filename to save the solution (will be saved in the test/output folder): ";
        cin >> outputFilename;
        outputFilename = "../test/output/" + outputFilename;
        ofstream outputFile(outputFilename);
        if (!outputFile.is_open())
        {
            cerr << "Unable to open file " << outputFilename << endl;
            return 1;
        }
        outputFile << currPrizeMax << endl;
        for (const auto &token : maxSequence.tokens)
        {
            outputFile << token.identifier << " ";
        }
        outputFile << endl;
        for (const auto &token : maxSequence.tokens)
        {
            outputFile << "(" << token.position.row + 1 << "," << token.position.col + 1 << ")" << endl;
        }
        outputFile << diff.count() << " ms";
        outputFile.close();
        cout << "Solution saved." << endl;
    }

    return 0;
}
