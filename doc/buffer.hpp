#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <vector>
#include <string>

class Buffer {
private:
    std::vector<std::string> buffer;
    int bufferSize;

public:
    Buffer(int size);
    void push(const std::string &element);
    void printBuffer();
    int getBufferSize();
};

#endif
