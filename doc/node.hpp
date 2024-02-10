#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <string>
#include "codematrix.hpp"

class Node
{
private:
    std::pair<int, int> position;
    std::vector<std::pair<int, int>> path;
    const CodeMatrix& state;

public:
    Node(const std::pair<int, int>& pos, const CodeMatrix& currentState);
    Node(const std::pair<int, int>& pos, const CodeMatrix& currentState, const std::vector<std::pair<int, int>>& currentPath);
    const CodeMatrix& getState() const;
    const std::pair<int, int>& getPosition() const;
    void appendPath(const std::pair<int, int>& waypoint);
    const std::vector<std::pair<int, int>>& getPath() const;
};

#endif
