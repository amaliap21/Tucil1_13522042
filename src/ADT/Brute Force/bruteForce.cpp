#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Structure to represent a single position in the matrix
struct Position
{
    int row;
    int col;
};

// Data structure to represent a sequence
struct Sequence
{
    string sequence;
    int reward;
};

// Data structure to store the loaded data from the input file
struct InputData
{
    int bufferSize;
    vector<vector<string>> matrix;
    vector<Sequence> sequences;
};

class SolutionFinder
{
private:
    InputData data;
    int bestReward;
    vector<Position> bestPath;
    vector<vector<string>> buffer;
    unordered_map<string, int> bufferRewards;
    milliseconds duration;

    bool isValidPosition(int row, int col)
    {
        return row >= 0 && row < data.matrix.size() && col >= 0 && col < data.matrix[0].size();
    }

    void dfs(int row, int col, int index, int currentReward, vector<Position> &currentPath, vector<vector<bool>> &visited)
    {
        // Base case: if all sequences are covered or we reach the end of the buffer
        if (index == data.bufferSize)
        {
            // Check if the current reward is better than the best reward so far
            if (currentReward > bestReward)
            {
                bestReward = currentReward;
                bestPath = currentPath;
            }
            return;
        }

        // Define possible movements: vertical and horizontal
        const vector<int> dr = {-1, 0, 1, 0}; // Vertically
        const vector<int> dc = {0, 1, 0, -1}; // Horizontally

        // Traverse the matrix using DFS
        for (int i = 0; i < 4; ++i)
        {
            int newRow = row + dr[i];
            int newCol = col + dc[i];
            if (isValidPosition(newRow, newCol) && !visited[newRow][newCol])
            {
                visited[newRow][newCol] = true;
                currentPath.push_back({newRow, newCol});

                // Extend the current sequence in all four directions
                string currentSeq;
                for (int k = index; k < data.bufferSize; ++k)
                {
                    currentSeq += data.matrix[currentPath[k].row][currentPath[k].col];
                    for (const auto &seq : data.sequences)
                    {
                        if (currentSeq.size() <= seq.sequence.size() && currentSeq == seq.sequence.substr(0, currentSeq.size()))
                        {
                            buffer[k].push_back(currentSeq);         // Store the current sequence in the buffer
                            bufferRewards[currentSeq] += seq.reward; // Store the reward for the current sequence
                            dfs(newRow, newCol, index + 1, bufferRewards[currentSeq], currentPath, visited);
                            buffer[k].pop_back();                    // Backtrack: remove the last sequence from the buffer
                            bufferRewards[currentSeq] -= seq.reward; // Remove the reward for the current sequence
                        }
                    }
                    newRow += dr[i];
                    newCol += dc[i];
                    if (!isValidPosition(newRow, newCol) || visited[newRow][newCol])
                    {
                        break;
                    }
                }

                // Backtrack
                visited[newRow - dr[i]][newCol - dc[i]] = false;
                currentPath.pop_back();
            }
        }
    }

public:
    SolutionFinder(const InputData &inputData) : data(inputData), bestReward(0), buffer(inputData.bufferSize, vector<string>()), bufferRewards(), duration(0) {}

    void findOptimalSolution()
    {
        auto start = high_resolution_clock::now();

        // Initialize variables to store the best solution
        vector<vector<bool>> visited(data.matrix.size(), vector<bool>(data.matrix[0].size(), false));
        vector<Position> currentPath;

        // Perform DFS from each position in the matrix
        for (size_t i = 0; i < data.matrix.size(); ++i)
        {
            for (size_t j = 0; j < data.matrix[i].size(); ++j)
            {
                visited[i][j] = true;
                currentPath.push_back({static_cast<int>(i), static_cast<int>(j)});
                dfs(i, j, 0, 0, currentPath, visited);
                visited[i][j] = false;
                currentPath.pop_back();
            }
        }

        auto stop = high_resolution_clock::now();
        duration = duration_cast<milliseconds>(stop - start);
    }

    void printSolution() const
    {
        // Output the solution
        cout << bestReward << endl;
        for (const auto &seq : buffer)
        {
            for (const auto &charSeq : seq)
            {
                cout << charSeq << " ";
            }
        }
        cout << endl;
        cout << "Sequence Positions:" << endl;
        for (const auto &pos : bestPath)
        {
            cout << "(" << pos.row + 1 << ", " << pos.col + 1 << ")" << endl;
        }
        cout << "Execution Time: " << duration.count() << " ms" << endl;
    }

    void saveSolutionToFile(const string &filename) const
    {
        // Save solution to output file
        ofstream outFile(filename);
        if (!outFile.is_open())
        {
            cerr << "Error opening file." << endl;
            return;
        }
        outFile << bestReward << endl;
        for (const auto &seq : buffer)
        {
            for (const auto &charSeq : seq)
            {
                outFile << charSeq << " ";
            }
        }
        outFile << endl;
        outFile << "Sequence Positions:" << endl;
        for (const auto &pos : bestPath)
        {
            outFile << "(" << pos.row + 1 << ", " << pos.col + 1 << ")" << endl;
        }
        outFile << "Execution Time: " << duration.count() << " ms" << endl;
        outFile.close();
    }
};

InputData parseInput(const string &filename)
{
    ifstream file(filename);
    InputData data;

    if (!file.is_open())
    {
        cerr << "Error opening file." << endl;
        exit(1);
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

int main()
{
    string filename = "load.txt"; // Update with your input file name
    InputData data = parseInput(filename);
    SolutionFinder solutionFinder(data);
    solutionFinder.findOptimalSolution();
    solutionFinder.printSolution();
    solutionFinder.saveSolutionToFile("output.txt"); // Save solution to output file
    return 0;
}
