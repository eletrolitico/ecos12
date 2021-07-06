#include <iostream>

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

int main()
{
    asio::error_code ec;
    asio::io_context context;

    std::cout << "Hello there" << std::endl;
    return 0;
}