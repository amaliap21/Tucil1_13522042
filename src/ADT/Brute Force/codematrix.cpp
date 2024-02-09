#include "codematrix.hpp"
#include <iostream>

CodeMatrix::CodeMatrix(int size) : matrix(size, std::vector<std::string>(size, "0")) {}

void CodeMatrix::printMatrix() const
{
    for (const auto &row : matrix)
    {
        for (const auto &elem : row)
        {
            std::cout << elem << "\t";
        }
        std::cout << std::endl;
    }
}

void CodeMatrix::fillMatrix(const std::vector<std::vector<std::string>> &inputMatrix)
{
    matrix = inputMatrix;
}

const std::vector<std::vector<std::string>> &CodeMatrix::getMatrix() const
{
    return matrix;
}
