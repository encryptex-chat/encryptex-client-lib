#include <future>
#include <iostream>

#include "CliSession.hpp"
#include "ClientApi.hpp"
#include "ServerConnection.hpp"
#include "common.hpp"

int main()
{
    etex::ServerConnection server_connection;
    etex::ClientApi client(server_connection);
    etex::CliSession cli_session(client);

    auto asio_thread =
        std::async(std::launch::async, &etex::ServerConnection::connect, &server_connection);

    client.join_service();

    cli_session.run();

    asio_thread.wait();

    return 0;
}