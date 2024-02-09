#ifndef CODEMATRIX_HPP
#define CODEMATRIX_HPP

#include <vector>
#include <string>

class CodeMatrix
{
private:
    std::vector<std::vector<std::string>> matrix;

public:
    CodeMatrix(int size);
    void printMatrix() const;
    void fillMatrix(const std::vector<std::vector<std::string>> &inputMatrix);
    const std::vector<std::vector<std::string>> &getMatrix() const; // Declare getMatrix() as const
};

#endif // CODEMATRIX_H
