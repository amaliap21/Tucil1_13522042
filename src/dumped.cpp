#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace std::chrono;

// Structure to represent a single position in the matrix
struct Position
{
    int row;
    int col;
};

// Data structure to store information about sequences
struct Sequence
{
    string sequence;
    int reward;
};

// Data structure to store the loaded data from input file
struct InputData
{
    int bufferSize;
    vector<vector<string>> matrix;
    vector<Sequence> sequences;
};

// Function to parse the input file and load the data into appropriate variables
InputData parseInput(const string &filename)
{
    ifstream file(filename);
    InputData data;

    if (!file.is_open())
    {
        cerr << "Error opening file." << endl;
            ;
    }

    // Read buffer size
    file >> data.bufferSize;

    // Read matrix dimensions and matrix itself
    int width, height;
    file >> width >> height;
    data.matrix.resize(height, vector<string>(width));
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            file >> data.matrix[i][j];
        }
    }

    // Read number of sequences
    int numSequences;
    file >> numSequences;

    // Read sequences and their rewards
    for (int i = 0; i < numSequences; ++i)
    {
        string sequence;
        int reward;
        file >> sequence >> reward;
        data.sequences.push_back({sequence, reward});
    }

    file.close();
    return data;
}

// Function to check if a given position is valid in the matrix
bool isValidPosition(int row, int col, int numRows, int numCols)
{
    return row >= 0 && row < numRows && col >= 0 && col < numCols;
}

// Function to perform a depth-first search to match sequences starting from a given position
void dfs(const vector<vector<string>> &matrix, const vector<Sequence> &sequences, int bufferSize, int row, int col, vector<Position> &path, vector<vector<bool>> &visited, vector<vector<bool>> &usedSequences, vector<Position> &bestPath, int &bestReward, int &totalSteps)
{
    if (path.size() == bufferSize || path.size() == sequences[0].sequence.size())
    {
        // Calculate reward for the current path
        int reward = 0;
        for (const auto &pos : path)
        {
            reward += sequences[matrix[pos.row][pos.col][0] - '0'].reward;
        }

        // Update best reward and path if the current path is better
        if (reward > bestReward || (reward == bestReward && path.size() < totalSteps))
        {
            bestReward = reward;
            bestPath = path;
            totalSteps = path.size();
        }
        return;
    }

    visited[row][col] = true;
    path.push_back({row, col});

    // Define possible movements: alternate between vertical and horizontal
    const vector<int> dr = {-1, 0, 1, 0}; // vertical then horizontal
    const vector<int> dc = {0, 1, 0, -1};

    // Determine the direction to start (vertical or horizontal)
    int startDir = path.size() % 2; // 0 for vertical, 1 for horizontal

    for (int i = 0; i < 4; ++i)
    {
        int dir = (startDir + i) % 4;
        int newRow = row + dr[dir];
        int newCol = col + dc[dir];
        if (isValidPosition(newRow, newCol, matrix.size(), matrix[0].size()) && !visited[newRow][newCol])
        {
            string sequenceSoFar;
            for (const auto &pos : path)
            {
                sequenceSoFar += matrix[pos.row][pos.col];
            }
            sequenceSoFar += matrix[newRow][newCol];

            // Check if the current sequence is a valid one
            for (const auto &seq : sequences)
            {
                if (sequenceSoFar.find(seq.sequence) == 0 && !usedSequences[path.size()][seq.sequence[0] - '0'])
                {
                    usedSequences[path.size()][seq.sequence[0] - '0'] = true;
                    dfs(matrix, sequences, bufferSize, newRow, newCol, path, visited, usedSequences, bestPath, bestReward, totalSteps);
                    usedSequences[path.size()][seq.sequence[0] - '0'] = false;
                }
            }
        }
    }

    path.pop_back();
    visited[row][col] = false;
}

// Function to find the optimal solution
void findOptimalSolution(const string &filename)
{
    InputData data = parseInput(filename);

    int numRows = data.matrix.size();
    int numCols = data.matrix[0].size();

    vector<vector<bool>> visited(numRows, vector<bool>(numCols, false));
    vector<vector<bool>> usedSequences(data.bufferSize + 1, vector<bool>(100, false)); // Assuming two-digit alphanumeric sequences

    auto start = high_resolution_clock::now();

    // Initialize variables to store the best solution
    int bestReward = 0;
    int totalSteps = 0;
    vector<Position> bestPath;

    // Perform DFS from each position in the matrix
    for (int i = 0; i < numRows; ++i)
    {
        for (int j = 0; j < numCols; ++j)
        {
            // Reset variables for each search
            vector<Position> currentPath;
            dfs(data.matrix, data.sequences, data.bufferSize, i, j, currentPath, visited, usedSequences, bestPath, bestReward, totalSteps);
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    // Output the solution
    cout << "Total bobot hadiah: " << bestReward << " poin" << endl;
    cout << "Total langkah: " << totalSteps << " langkah" << endl;
    cout << "Solusi optimal: ";
    for (const auto &pos : bestPath)
    {
        cout << "(" << pos.row + 1 << ", " << pos.col + 1 << ") ";
    }
    cout << endl;
    cout << "Waktu eksekusi program: " << duration.count() << " ms" << endl;

    // Prompt for saving solution to a file
    cout << "Apakah ingin menyimpan solusi? (y/n)" << endl;
    char choice;
    cin >> choice;
    if (choice == 'y' || choice == 'Y')
    {
        // Save solution to output file
        ofstream outputFile("output.txt");
        if (outputFile.is_open())
        {
            outputFile << bestReward << endl;
            if (bestReward > 0)
            {
                for (const auto &pos : bestPath)
                {
                    outputFile << pos.row + 1 << ", " << pos.col + 1 << endl;
                }
            }
            outputFile << "Waktu eksekusi program: " << duration.count() << " ms" << endl;
            outputFile.close();
            cout << "Solusi berhasil disimpan dalam file output.txt" << endl;
        }
        else
        {
            cout << "Error: Gagal menyimpan solusi ke file." << endl;
        }
    }
}

int main()
{
    findOptimalSolution("load.txt");
    return 0;
}