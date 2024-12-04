#include <future>
#include <iostream>

#include "ClientApi.hpp"
#include "ServerConnection.hpp"

int main()
{
    etex::ServerConnection server_connection;
    etex::ClientApi client(server_connection);
    auto connection_to_server_thread =
        std::async(std::launch::async, &etex::ServerConnection::connect, &server_connection);

    client.join_service();
    // server_connection.send_message("Test message to server\n");

    connection_to_server_thread.wait();

    return 0;
}