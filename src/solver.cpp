#include "solver.hpp"
#include "node.hpp"
#include <algorithm>
#include <chrono>
#include <tuple>
#include <vector>
#include <numeric>
#include <iostream>

std::vector<std::vector<std::pair<int, int>>> traverse_matrix(const std::vector<std::vector<std::string>> &matrix, int buffer_size)
{
    // Implementation of traverse_matrix
    std::vector<std::vector<std::pair<int, int>>> paths;
    // Sample implementation, assuming the matrix is a grid of strings representing locations
    for (int i = 0; i < matrix.size(); ++i)
    {
        for (int j = 0; j < matrix[i].size(); ++j)
        {
            if (matrix[i][j] == "X")
                continue; // Skip visited locations
            std::vector<std::pair<int, int>> path;
            path.push_back(std::make_pair(i, j));
            paths.push_back(path);
        }
    }
    return paths;
}

std::vector<std::pair<int, int>> get_possible_movement(const std::vector<std::vector<std::string>> &matrix, const std::pair<int, int> &position, bool direction)
{
    // Implementation of get_possible_movement
    std::vector<std::pair<int, int>> possible_movement;
    // Sample implementation, assuming only vertical and horizontal movements
    if (direction)
    {
        // Vertical movement
        if (position.first - 1 >= 0 && matrix[position.first - 1][position.second] != "X")
        {
            possible_movement.push_back(std::make_pair(position.first - 1, position.second));
        }
        if (position.first + 1 < matrix.size() && matrix[position.first + 1][position.second] != "X")
        {
            possible_movement.push_back(std::make_pair(position.first + 1, position.second));
        }
    }
    else
    {
        // Horizontal movement
        if (position.second - 1 >= 0 && matrix[position.first][position.second - 1] != "X")
        {
            possible_movement.push_back(std::make_pair(position.first, position.second - 1));
        }
        if (position.second + 1 < matrix[position.first].size() && matrix[position.first][position.second + 1] != "X")
        {
            possible_movement.push_back(std::make_pair(position.first, position.second + 1));
        }
    }
    return possible_movement;
}

std::vector<std::vector<std::string>> mark_visited(const std::vector<std::vector<std::string>> &matrix, const std::vector<std::pair<int, int>> &places)
{
    // Implementation of mark_visited
    std::vector<std::vector<std::string>> new_matrix = matrix;
    for (const auto &place : places)
    {
        new_matrix[place.first][place.second] = "X"; // Mark visited location
    }
    return new_matrix;
}

std::vector<std::string> check_sequences_in_combinations(const std::vector<std::vector<std::string>> &sequences, const std::vector<std::vector<std::string>> &combinations, int buffer_size)
{
    // Implementation of check_sequences_in_combinations
    std::vector<std::string> result;
    // Sample implementation, assuming sequences and combinations are vectors of strings
    for (const auto &combination : combinations)
    {
        bool present = false;
        for (const auto &sequence : sequences)
        {
            if (std::search(combination.begin(), combination.end(), sequence.begin(), sequence.end()) != combination.end())
            {
                present = true;
                break;
            }
        }
        result.push_back(present ? "True" : "False");
    }
    return result;
}

std::vector<std::vector<std::string>> generate_combinations(const std::vector<std::vector<std::string>> &sequences, int buffer_size)
{
    // Implementation of generate_combinations
    std::vector<std::vector<std::string>> combinations;
    // Sample implementation, generating all possible combinations of sequences
    std::vector<int> indices(sequences.size());
    std::iota(indices.begin(), indices.end(), 0); // Initialize indices as 0, 1, 2, ..., sequences.size() - 1
    do
    {
        std::vector<std::string> combination;
        for (const auto &index : indices)
        {
            combination.insert(combination.end(), sequences[index].begin(), sequences[index].end());
        }
        combinations.push_back(combination);
    } while (std::next_permutation(indices.begin(), indices.end()));
    return combinations;
}

std::vector<std::vector<std::pair<int, int>>> compare_paths_and_combinations(const std::vector<std::vector<std::string>> &matrix, const std::vector<std::vector<std::pair<int, int>>> &paths, const std::vector<std::vector<std::string>> &picked_sequences, bool only_one)
{
    // Implementation of compare_paths_and_combinations
    std::vector<std::vector<std::pair<int, int>>> selected_paths;
    // Sample implementation, comparing paths and combinations
    for (const auto &path : paths)
    {
        std::string path_str;
        for (const auto &coord : path)
        {
            path_str += matrix[coord.first][coord.second];
        }
        for (const auto &sequence : picked_sequences)
        {
            if (path_str.find(sequence[0]) != std::string::npos)
            {
                selected_paths.push_back(path);
                if (only_one)
                    return selected_paths; // Return if only one path is needed
                break;
            }
        }
    }
    return selected_paths;
}

int calculate_reward(const std::vector<std::vector<std::pair<int, int>>> &selected_path, const std::vector<int> &sequence_rewards)
{
    // Implementation of calculate_reward
    int reward = 0;
    // Sample implementation, summing rewards for visited locations
    for (const auto &path : selected_path)
    {
        for (const auto &coord : path)
        {
            reward += sequence_rewards[coord.first * selected_path[0].size() + coord.second]; // Assuming sequence_rewards is a 1D array
        }
    }
    return reward;
}

double calculate_duration(std::chrono::time_point<std::chrono::high_resolution_clock> start)
{
    // Implementation of calculate_duration
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

std::tuple<std::vector<std::vector<std::string>>, std::vector<std::vector<std::pair<int, int>>>, int, double> Solver::solve(int buffer, const std::vector<std::vector<std::string>> &codeMatrix, const std::vector<std::vector<std::string>> &sequences, const std::vector<int> &sequenceRewards)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::vector<std::string>> bestBuffer;
    std::vector<std::vector<std::pair<int, int>>> bestPath;
    int bestReward = 0;

    std::vector<std::vector<std::string>> combinations = generate_combinations(sequences, buffer);

    std::vector<std::string> possible_combinations = check_sequences_in_combinations(sequences, combinations, buffer);

    std::vector<std::vector<std::pair<int, int>>> paths = traverse_matrix(codeMatrix, buffer);

    std::vector<std::vector<std::pair<int, int>>> selected_path = compare_paths_and_combinations(codeMatrix, paths, combinations, true);

    bestReward = calculate_reward(selected_path, sequenceRewards);

    double duration = calculate_duration(start);

    return std::make_tuple(bestBuffer, selected_path, bestReward, duration);
}