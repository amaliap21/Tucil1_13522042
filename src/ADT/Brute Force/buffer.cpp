#include "buffer.hpp"
#include <iostream>

Buffer::Buffer(int size) : bufferSize(size)
{
    buffer.resize(size);
}

void Buffer::push(const std::string &element)
{
    if (buffer.size() < bufferSize)
    {
        buffer.insert(buffer.begin(), element);
        buffer.pop_back();
    }
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