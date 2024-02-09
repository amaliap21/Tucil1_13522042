#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

#include "../Save-Load/save-load.cpp" // include file save-load.hpp

using namespace std;
using namespace std::chrono;

// Structure to represent a single position in the matrix
struct Position
{
    int row;
    int col;
};

// Function to parse the input file and load the data into appropriate variables
DataLoad parseInput(const string &filename)
{
    ifstream file(filename);
    DataLoad data;

    if (!file.is_open())
    {
        cerr << "Error opening file." << endl;
        exit(1);
    }

    string line;
    // Membaca ukuran buffer
    getline(file, line);
    stringstream(line) >> data.ukuranBuffer;

    // Membaca ukuran matriks
    getline(file, line);
    stringstream ss(line);
    Ukuran ukuran;
    ss >> ukuran.baris >> ukuran.kolom;
    data.ukuranMatriks.push_back(ukuran);

    // Membaca isi matriks
    for (int i = 0; i < ukuran.baris; ++i)
    {
        getline(file, line);
        stringstream ss(line);
        vector<string> row;
        string token;

        for (int j = 0; j < ukuran.kolom; ++j)
        {
            ss >> token;
            row.push_back(token);
        }
        data.matriks.push_back(row);
    }

    // Membaca jumlah sekuens
    getline(file, line);
    stringstream(line) >> data.numberOfSequences;

    // Membaca sekuens dan bobot
    for (int i = 0; i < data.numberOfSequences; ++i)
    {
        getline(file, line); // Membaca sekuens
        data.sequences.push_back(line);

        getline(file, line); // Membaca bobot
        int bobot;
        stringstream(line) >> bobot;
        data.sequenceReward.push_back(bobot);
    }

    file.close();
    cout << "Data berhasil dimuat dari " << filename << endl;
    return data;
}

// Function to check if a given position is valid in the matrix
bool isValidPosition(int row, int col, int matrixSize)
{
    return row >= 0 && row < matrixSize && col >= 0 && col < matrixSize;
}

// Function to perform a depth-first search to find all sequences starting from a given position
void dfs(vector<vector<string>> &matrix, vector<string> &sequences, int bufferSize, int row, int col, vector<Position> &path, vector<vector<bool>> &visited, vector<vector<bool>> &usedSequences, vector<vector<Position>> &solution)
{
    if (path.size() == bufferSize)
    {
        solution.push_back(path);
        return;
    }

    visited[row][col] = true;
    path.push_back({row, col});

    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            if (i == 0 && j == 0)
                continue;
            int newRow = row + i;
            int newCol = col + j;
            if (isValidPosition(newRow, newCol, matrix.size()) && !visited[newRow][newCol])
            {
                string sequenceSoFar = matrix[newRow][newCol];
                for (const auto &pos : path)
                {
                    sequenceSoFar += matrix[pos.row][pos.col];
                }
                for (const auto &seq : sequences)
                {
                    if (sequenceSoFar.find(seq) != string::npos && !usedSequences[path.size()][seq[0] - '0'])
                    {
                        usedSequences[path.size()][seq[0] - '0'] = true;
                        dfs(matrix, sequences, bufferSize, newRow, newCol, path, visited, usedSequences, solution);
                        usedSequences[path.size()][seq[0] - '0'] = false;
                    }
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
    DataLoad data = parseInput(filename);

    vector<vector<Position>> solution;
    vector<vector<bool>> visited(data.ukuranMatriks[0].baris, vector<bool>(data.ukuranMatriks[0].kolom, false));
    vector<vector<bool>> usedSequences(data.ukuranBuffer + 1, vector<bool>(10, false)); // Assuming single digit sequences

    auto start = high_resolution_clock::now();

    // Perform DFS from each position in the matrix
    for (int i = 0; i < data.ukuranMatriks[0].baris; ++i)
    {
        for (int j = 0; j < data.ukuranMatriks[0].kolom; ++j)
        {
            vector<Position> path;
            dfs(data.matriks, data.sequences, data.ukuranBuffer, i, j, path, visited, usedSequences, solution);
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    // Find the solution with the highest reward
    int maxReward = 0;
    int maxIdx = -1;
    for (int i = 0; i < solution.size(); ++i)
    {
        int reward = 0;
        for (const auto &pos : solution[i])
        {
            reward += data.sequenceReward[pos.row * data.ukuranMatriks[0].kolom + pos.col];
        }
        if (reward > maxReward)
        {
            maxReward = reward;
            maxIdx = i;
        }
    }

    // Output the solution
    cout << "Output (solusi):" << endl;
    cout << maxReward << endl; // Output the maximum reward
    // Output the buffer
    for (const auto &pos : solution[maxIdx])
    {
        cout << data.matriks[pos.row][pos.col] << " ";
    }
    cout << endl;
    // Output the coordinates
    for (const auto &pos : solution[maxIdx])
    {
        cout << pos.row + 1 << ", " << pos.col + 1 << endl;
    }
    cout << endl
         << duration.count() << " ms" << endl;

    // Ask if the solution should be saved
    char save;
    cout << "Apakah ingin menyimpan solusi? (y/n)" << endl;
    cin >> save;
    if (save == 'y' || save == 'Y')
    {
        // Save the solution here
        DataSave dataToSave;
        dataToSave.bobotHadiah = maxReward; // Use the maximum reward
        // Use the buffer from the optimal solution
        for (const auto &pos : solution[maxIdx])
        {
            dataToSave.isiBuffer += data.matriks[pos.row][pos.col] + " ";
        }
        // Use the coordinates from the optimal solution
        for (const auto &pos : solution[maxIdx])
        {
            dataToSave.koordinatList.push_back({pos.row + 1, pos.col + 1}); // Adjust row and col indices to start from 1
        }
        // Simpan data ke file
        string fileSave;
        cout << "Masukkan nama file untuk disimpan: ";
        cin >> fileSave;
        saveData("../test/" + fileSave, dataToSave, duration.count()); // Menyimpan durasi eksekusi
    }
}


void dfs(vector<vector<string>> &matrix, vector<string> &sequences, int bufferSize, int row, int col, vector<Position> &path, vector<vector<bool>> &visited, vector<vector<bool>> &usedSequences, vector<vector<Position>> &solution, int &maxReward, DataLoad &data)
{
    if (path.size() == bufferSize)
    {
        int reward = 0;
        for (const auto &pos : path)
        {
            reward += data.sequenceReward[pos.row * data.ukuranMatriks[0].kolom + pos.col];
        }
        if (reward > maxReward)
        {
            maxReward = reward;
            solution.clear();
            solution.push_back(path);
        }
        else if (reward == maxReward)
        {
            solution.push_back(path);
        }
        return;
    }

    visited[row][col] = true;
    path.push_back({row, col});

    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            if (i == 0 && j == 0)
                continue;
            int newRow = row + i;
            int newCol = col + j;
            if (isValidPosition(newRow, newCol, matrix.size()) && !visited[newRow][newCol])
            {
                string sequenceSoFar = matrix[newRow][newCol];
                for (const auto &pos : path)
                {
                    sequenceSoFar += matrix[pos.row][pos.col];
                }
                for (const auto &seq : sequences)
                {
                    if (sequenceSoFar.find(seq) != string::npos && !usedSequences[path.size()][seq[0] - '0'])
                    {
                        usedSequences[path.size()][seq[0] - '0'] = true;
                        dfs(matrix, sequences, bufferSize, newRow, newCol, path, visited, usedSequences, solution, maxReward, data);
                        usedSequences[path.size()][seq[0] - '0'] = false;
                    }
                }
            }
        }
    }

    path.pop_back();
    visited[row][col] = false;
}

int main()
{
    findOptimalSolution("load.txt");
    return 0;
}
