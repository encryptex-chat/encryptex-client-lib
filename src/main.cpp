#include <future>
#include <iostream>

#include "ServerConnection.hpp"

int main()
{
    etex::ServerConnection server_connection;
    auto fut = std::async(std::launch::async, &etex::ServerConnection::connect, &server_connection);

    server_connection.send_message("Test message to server\n");

    fut.wait();

    return 0;
}