#include <boost/asio.hpp>
#include <iostream>

#include "message.hpp"

using boost::asio::ip::tcp;

constexpr bool k_async_write{true};

void write_handler(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    std::cout << error.what() << ", bytes transfered: " << bytes_transferred << std::endl;
}

int main()
{
    try
    {
        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve(
            etex::common::k_server_addr, std::to_string(etex::common::k_server_port));

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        const std::string msg{"Hello, bitches!\n"};

        if (k_async_write)
        {
            socket.async_write_some(boost::asio::buffer(msg), &write_handler);
            io_context.run();
        } else
        {
            const auto bytes_written = socket.write_some(boost::asio::buffer(msg));
            std::cout << "Written " << bytes_written << " bytes" << std::endl;
        }

    } catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}