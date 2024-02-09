#include "node.hpp"

Node::Node(const std::pair<int, int>& pos, const CodeMatrix& currentState) : position(pos), state(currentState) {}

Node::Node(const std::pair<int, int>& pos, const CodeMatrix& currentState, const std::vector<std::pair<int, int>>& currentPath) : position(pos), state(currentState), path(currentPath) {}

const CodeMatrix& Node::getState() const {
    return state;
}

const std::pair<int, int>& Node::getPosition() const {
    return position;
}

void Node::appendPath(const std::pair<int, int>& waypoint) {
    path.push_back(waypoint);
}

const std::vector<std::pair<int, int>>& Node::getPath() const {
    return path;
}
