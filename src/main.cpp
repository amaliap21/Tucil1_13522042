// main.cpp
#include "helpers.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

void writeToFile(const string &filename, int buffer_size, int matrix_width, int matrix_height, const vector<vector<string>> &matrix, int number_of_sequences, const vector<vector<string>> &sequences, const vector<vector<int>> &sequence_rewards)
{
    ofstream file(filename);
    if (!file.is_open())
    {
        cerr << "Failed to open file for writing." << endl;
        return;
    }

    file << buffer_size << endl;
    file << matrix_width << " " << matrix_height << endl;

    for (const auto &row : matrix)
    {
        for (const auto &token : row)
        {
            file << token << " ";
        }
        file << endl;
    }

    file << number_of_sequences << endl;
    for (int i = 0; i < number_of_sequences; ++i)
    {
        for (const auto &token : sequences[i])
        {
            file << token << " ";
        }
        file << endl;

        // Menulis reward sequence (hanya sekali) dengan angka acak antara -100 hingga 100
        int random_reward = rand() % 201 - 100; // Menghasilkan angka acak dari -100 hingga 100
        file << random_reward << " ";

        if (i < number_of_sequences - 1)
        {
            file << endl;
        }
    }

    file.close();
}

void randomizer(int jumlah_token_unik, const vector<string> &token, int ukuran_buffer, int width, int height, int jumlah_sekuens, int ukuran_maksimal_sekuens)
{
    // Seed the random number generator
    srand(time(NULL));

    // Matriks random
    vector<vector<string>> matrix(height, vector<string>(width));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            matrix[i][j] = token[rand() % jumlah_token_unik];
        }
    }

    // Sekuens random
    vector<vector<string>> sequences(jumlah_sekuens, vector<string>(ukuran_maksimal_sekuens));
    vector<vector<int>> sequence_rewards(jumlah_sekuens, vector<int>(ukuran_maksimal_sekuens));
    for (int i = 0; i < jumlah_sekuens; i++)
    {
        for (int j = 0; j < ukuran_maksimal_sekuens; j++)
        {
            sequences[i][j] = token[rand() % jumlah_token_unik];
            sequence_rewards[i][j] = rand() % 201 - 100; // nilai -100 hingga 100
        }
    }

    writeToFile("random.txt", ukuran_buffer, width, height, matrix, jumlah_sekuens, sequences, sequence_rewards);
}

void randomMatrixSequence()
{
    int jumlah_token_unik;

    cout << "Masukkan jumlah token unik: ";
    cin >> jumlah_token_unik;

    cout << "Masukkan token: ";
    vector<string> tokens(jumlah_token_unik);
    for (int i = 0; i < jumlah_token_unik; i++)
    {
        cin >> tokens[i];
    }

    int ukuran_buffer, width, height, jumlah_sekuens, ukuran_maksimal_sekuens;

    cout << "Masukkan ukuran buffer: ";
    cin >> ukuran_buffer;

    cout << "Masukkan ukuran matriks (width height): ";
    cin >> width >> height;

    cout << "Masukkan jumlah sekuens: ";
    cin >> jumlah_sekuens;

    cout << "Masukkan ukuran maksimal sekuens: ";
    cin >> ukuran_maksimal_sekuens;

    randomizer(jumlah_token_unik, tokens, ukuran_buffer, width, height, jumlah_sekuens, ukuran_maksimal_sekuens);
}

vector<vector<string>> readMatrix(ifstream &inputFile, int width, int height)
{
    vector<vector<string>> matrix;
    string line;
    for (int i = 0; i < height; ++i)
    {
        getline(inputFile, line);
        istringstream iss(line);
        vector<string> row;
        for (int j = 0; j < width; ++j)
        {
            string token;
            iss >> token;
            row.push_back(token);
        }
        matrix.push_back(row);
    }
    return matrix;
}

vector<Sequence> readSequences(ifstream &inputFile, int numberOfSequences)
{
    vector<Sequence> sequences;
    for (int i = 0; i < numberOfSequences; ++i)
    {
        string line, token;
        int reward;
        getline(inputFile, line);
        istringstream iss(line);
        vector<string> tokens;
        while (iss >> token)
        {
            tokens.push_back(token);
        }
        inputFile >> reward;
        inputFile.ignore(); // Ignore newline character after reading reward
        sequences.emplace_back(tokens, reward);
    }
    return sequences;
}

int main(int argc, char *argv[])
{
    string filename; // Example filename, change as needed

    cout << "Apakah ingin menggunakan randomizer untuk matrix dan sequences? (y/n): ";

    string isRandom;
    cin >> isRandom;

    if (isRandom == "y" || isRandom == "Y")
    {
        randomMatrixSequence();
        filename = "random.txt";
    }
    else
    {
        cout << "Masukkan nama file: ";
        cin >> filename;
    }

    if (argc > 1)
    {
        filename = argv[1]; // Allow filename to be passed as argument
    }

    ifstream inputFile(filename);
    if (!inputFile.is_open())
    {
        cerr << "Failed to open file: " << filename << endl;
        return 1;
    }

    int bufferSize, width, height, numberOfSequences;
    inputFile >> bufferSize >> width >> height;
    inputFile.ignore(); // Ignore newline character after reading header

    auto matrix = readMatrix(inputFile, width, height);

    inputFile >> numberOfSequences;
    inputFile.ignore(); // Ignore newline character after reading number of sequences

    auto sequences = readSequences(inputFile, numberOfSequences);

    BreachProtocolSolver solver(bufferSize, matrix, sequences);
    solver.solve();
    solver.printSolution();

    return 0;
}
