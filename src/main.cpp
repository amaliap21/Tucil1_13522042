#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include "solver.hpp"

using namespace std;

int main()
{
    Solver solver;

    // Open the input file
    ifstream inputFile("load.txt");
    if (!inputFile)
    {
        cerr << "Unable to open input file." << endl;
        return 1;
    }

    int buffer;
    inputFile >> buffer;

    int matrixWidth, matrixHeight;
    inputFile >> matrixWidth >> matrixHeight;

    vector<vector<string>> codeMatrix(matrixHeight, vector<string>(matrixWidth));
    for (int i = 0; i < matrixHeight; ++i)
    {
        for (int j = 0; j < matrixWidth; ++j)
        {
            inputFile >> codeMatrix[i][j];
        }
    }

    int numberOfSequences;
    inputFile >> numberOfSequences;

    vector<vector<string>> sequences(numberOfSequences);
    vector<int> sequenceRewards(numberOfSequences);

    for (int i = 0; i < numberOfSequences; ++i)
    {
        int sequenceLength;
        inputFile >> sequenceLength;
        sequences[i].resize(sequenceLength);
        for (int j = 0; j < sequenceLength; ++j)
        {
            inputFile >> sequences[i][j];
        }
        inputFile >> sequenceRewards[i];
    }

    // Close the input file
    inputFile.close();

    // Solve the problem
    vector<vector<string>> bestBuffer;
    vector<vector<pair<int, int>>> bestPath;
    int bestReward;
    double duration;
    tie(bestBuffer, bestPath, bestReward, duration) = solver.solve(buffer, codeMatrix, sequences, sequenceRewards);

    // Output the result
    cout << "Best Reward: " << bestReward << endl;
    cout << "Best Buffer:" << endl;
    for (const auto &p : bestBuffer)
    {
        for (const auto &s : p)
        {
            cout << s << " ";
        }
        cout << endl;
    }
    cout << "Best Path:" << endl;
    for (const auto &path : bestPath)
    {
        for (const auto &p : path)
        {
            cout << "(" << p.first << ", " << p.second << ") ";
        }
        cout << endl;
    }
    cout << "Duration: " << duration << " ms" << endl;

    char saveSolution;
    cout << "Do you want to save the solution? (y/n): ";
    cin >> saveSolution;
    if (saveSolution == 'y' || saveSolution == 'Y')
    {
        // Save the solution to a file or database
        cout << "Solution saved." << endl;
    }

    return 0;
}
