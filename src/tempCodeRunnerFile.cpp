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
int maxBuffer = 0;
Point maxCoordinate;
int maxSteps = 0;

// Function to check if a point is within matrix bounds
bool isValid(Point p, int rows, int cols)
{
    return p.row >= 0 && p.row < rows && p.col >= 0 && p.col < cols;
}

// Memeriksa apakah sequence adalah subset dari actual
bool isSubsetOf(Sequence prize, Sequence actual)
{
    for (int i = 0; i <= actual.length - prize.length; i++)
    {
        bool found = true;
        for (int j = 0; j < prize.length; j++)
        {
            if (prize.tokens[j].identifier != actual.tokens[i + j].identifier)
            {
                found = false;
                break;
            }
        }
        if (found)
            return true;
    }
    return false;
}

// Menghitung hadiah dari suatu urutan berdasarkan urutan yang diberikan
int evaluateSequence(Sequence sequence, vector<Sequence> sequences)
{
    int prize = 0;
    for (const auto &prizeSeq : sequences)
    {
        if (isSubsetOf(prizeSeq, sequence))
        {
            prize += prizeSeq.prize;
        }
    }
    return prize;
}

// Depth-First Search (DFS) function untuk mencari semua jalur yang mungkin dan mengevaluasi hadiah
// Depth-First Search (DFS) function untuk mencari semua jalur yang mungkin dan mengevaluasi hadiah
void dfs(Point current, int remainingSteps, Sequence currentSequence, vector<vector<bool>> visited, bool horizontalMove, int buffer)
{
    if (remainingSteps == 0)
    {
        int prize = evaluateSequence(currentSequence, prizeSequences);
        if (prize > currPrizeMax)
        {
            currPrizeMax = prize;
            maxSequence = currentSequence;
            maxBuffer = buffer;
            maxCoordinate = current;
            maxSteps = evaluateMatrix.rows * evaluateMatrix.cols - remainingSteps;
        }
    }
    else
    {
        // Explore all possible directions regardless of horizontalMove
        const vector<Point> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}}; // Right, Left, Down, Up
        for (const auto &dir : directions)
        {
            Point next = {current.row + dir.row, current.col + dir.col};
            if (isValid(next, evaluateMatrix.rows, evaluateMatrix.cols) && !visited[next.row][next.col])
            {
                visited[next.row][next.col] = true;
                Token nextToken = evaluateMatrix.elements[next.row][next.col];
                Sequence nextSequence = currentSequence;
                nextSequence.tokens.push_back(nextToken);
                int nextBuffer = buffer + 1;
                if (!isSubsetOf(prizeSequences, nextSequence))
                {
                    nextBuffer = 0;
                }
                dfs(next, remainingSteps - 1, nextSequence, visited, !horizontalMove, nextBuffer); // Change move type after each step
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
            cout << "(" << token.position.row + 1 << ", " << token.position.col + 1 << ")" << endl; // Print row, column
        }
        cout << "Buffer: " << maxBuffer << endl;
        cout << "Coordinate: (" << maxCoordinate.row + 1 << ", " << maxCoordinate.col + 1 << ")" << endl; // Print row, column
        cout << "Steps: " << maxSteps << endl;
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
            dfs({i, j}, bufferSize - 1, startSequence, visited, true, 0);
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
        outputFile << "Buffer: " << maxBuffer << endl;
        outputFile << "Coordinate: (" << maxCoordinate.row + 1 << "," << maxCoordinate.col + 1 << ")" << endl;
        outputFile << "Steps: " << maxSteps << endl;
        outputFile << diff.count() << " ms";
        outputFile.close();
        cout << "Solution saved." << endl;
    }

    return 0;
}
