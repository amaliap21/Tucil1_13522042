#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;

// Struct untuk merepresentasikan titik dalam matriks
struct Point
{
    int row;
    int col;
};

struct Token
{
    int identifier;
    pair<int, int> position;

    // Constructor to initialize Token
    Token(int id, pair<int, int> pos) : identifier(id), position(pos) {}
};

// Define struct Sequence
struct Sequence
{
    vector<Token> tokens;
    int length;
    int prize;

    // Constructor to initialize Sequence
    Sequence(const vector<Token> &tok, int len, int pr) : tokens(tok), length(len), prize(pr) {}
};

// Struct untuk merepresentasikan matriks token
struct Matrix
{
    vector<vector<Token>> elements;
    int rows;
    int cols;
};

// Fungsi untuk memeriksa apakah sebuah titik valid dalam matriks
bool isValid(Point p, int rows, int cols)
{
    return p.row >= 0 && p.row < rows && p.col >= 0 && p.col < cols;
}

// Fungsi untuk mengevaluasi hadiah dari sebuah sekuens token berdasarkan sekuens hadiah yang tersedia
int evaluateSequence(const Sequence &sequence, const vector<Sequence> &prizeSequences)
{
    int prize = 0;
    for (const auto &prizeSeq : prizeSequences)
    {
        for (size_t i = 0; i <= sequence.tokens.size() - prizeSeq.tokens.size(); ++i)
        {
            bool found = true;
            for (size_t j = 0; j < prizeSeq.tokens.size(); ++j)
            {
                if (prizeSeq.tokens[j].identifier != sequence.tokens[i + j].identifier)
                {
                    found = false;
                    break;
                }
            }
            if (found)
            {
                prize += prizeSeq.prize;
            }
        }
    }
    return prize;
}

// Fungsi rekursif untuk melakukan Depth-First Search (DFS) untuk menemukan jalur optimal
void dfs(const Point &current, int remainingSteps, const Sequence &currentSequence, const Matrix &evaluateMatrix, const vector<Sequence> &prizeSequences, Sequence &maxSequence, vector<vector<bool>> &visited)
{
    if (remainingSteps == 0)
    {
        int prize = evaluateSequence(currentSequence, prizeSequences);
        if (prize > maxSequence.prize)
        {
            maxSequence = currentSequence;
            maxSequence.prize = prize;
        }
    }
    else
    {
        // Daftar arah yang mungkin
        const vector<Point> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}}; // Right, Left, Down, Up
        for (const auto &dir : directions)
        {
            Point next = {current.row + dir.row, current.col + dir.col};
            if (isValid(next, evaluateMatrix.rows, evaluateMatrix.cols) && !visited[next.row][next.col])
            {
                visited[next.row][next.col] = true;
                const Token &nextToken = evaluateMatrix.elements[next.row][next.col];
                Sequence nextSequence = currentSequence;
                nextSequence.tokens.push_back(nextToken);
                dfs(next, remainingSteps - 1, nextSequence, evaluateMatrix, prizeSequences, maxSequence, visited);
                visited[next.row][next.col] = false;
            }
        }
    }
}

// Fungsi untuk mencetak hasil ke layar
void printResult(const Sequence &maxSequence, int diff)
{
    if (!maxSequence.tokens.empty())
    {
        cout << "Maximum reward: " << maxSequence.prize << endl;
        cout << "Sequence: ";
        for (const auto &token : maxSequence.tokens)
        {
            cout << token.identifier << " ";
        }
        cout << endl;
        cout << "Paths: " << endl;
        for (const auto &token : maxSequence.tokens)
        {
            cout << "(" << token.position.row + 1 << ", " << token.position.col + 1 << ")" << endl; // Print row, column
        }
        cout << endl;
        cout << "Time: " << diff << " ms" << endl;
    }
    else
    {
        cout << "There is no optimal solution for this problem :(" << endl;
    }
}

// Fungsi utama
int main()
{
    string filename;
    cout << "Enter the filename to read the matrix and sequences (relative to the test/input folder): ";
    cin >> filename;
    filename = "../test/input/" + filename;

    ifstream inputFile(filename);
    if (!inputFile.is_open())
    {
        cerr << "Unable to open file " << filename << endl;
        return 1;
    }

    // Baca buffer size, ukuran matriks, dan inisialisasi matriks
    int bufferSize;
    inputFile >> bufferSize;
    int rows, cols;
    inputFile >> rows >> cols;
    Matrix evaluateMatrix;
    evaluateMatrix.rows = rows;
    evaluateMatrix.cols = cols;
    evaluateMatrix.elements.resize(rows, vector<Token>(cols));

    // Baca matriks dari file
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            string identifier;
            inputFile >> identifier;
            evaluateMatrix.elements[i][j] = {identifier, {i, j}};
        }
    }

    // Baca jumlah sekuens hadiah dan daftar sekuens hadiah
    int numSequences;
    inputFile >> numSequences;
    vector<Sequence> prizeSequences(numSequences);
    for (int i = 0; i < numSequences; ++i)
    {
        int length;
        inputFile >> length;
        vector<Token> tokens(length);
        for (int j = 0; j < length; ++j)
        {
            string identifier;
            inputFile >> identifier;
            tokens[j] = {identifier, {-1, -1}}; // Row and col will be set during evaluation
        }
        int prize;
        inputFile >> prize;
        prizeSequences[i] = {tokens, length, prize};
    }

    inputFile.close();

    // Variables to store the maximum sequence found
    Sequence maxSequence = {{}, 0, 0};

    // Variables to store the duration of the search
    auto start = chrono::steady_clock::now();

    // DFS untuk mencari jalur optimal
    vector<vector<bool>> visited(rows, vector<bool>(cols, false)); // Inisialisasi matriks visited
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            Point start = {i, j};
            visited[i][j] = true;
            dfs(start, bufferSize - 1, {evaluateMatrix.elements[i][j]}, evaluateMatrix, prizeSequences, maxSequence, visited);
            visited[i][j] = false;
        }
    }

    // Hitung durasi pencarian
    auto end = chrono::steady_clock::now();
    auto diff = chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Cetak hasil
    printResult(maxSequence, diff.count());

    return 0;
}
