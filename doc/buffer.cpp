#include "buffer.hpp"
#include <iostream>

Buffer::Buffer(int size) : bufferSize(size)
{
    buffer.resize(size);
}

void Buffer::push(const std::string &element)
{
    if (buffer.size() == bufferSize)
    {
        buffer.pop_back(); // Remove the oldest element if the buffer is full
    }
    buffer.insert(buffer.begin(), element); // Add the new element at the beginning
}

void Buffer::printBuffer()
{
    for (const auto &element : buffer)
    {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

int Buffer::getBufferSize()
{
    return bufferSize;
}
