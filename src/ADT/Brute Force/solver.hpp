#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <vector>
#include <string>
#include <tuple>

class Solver
{
public:
    static std::tuple<std::vector<std::vector<std::string>>, std::vector<std::vector<std::pair<int, int>>>, int, double> solve(int buffer, const std::vector<std::vector<std::string>> &codeMatrix, const std::vector<std::vector<std::string>> &sequences, const std::vector<int> &sequenceRewards);
};

#endif
