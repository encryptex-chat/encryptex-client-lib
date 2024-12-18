#include <future>
#include <iostream>

#include "ClientApi.hpp"
#include "ServerConnection.hpp"
#include "common.hpp"

int main()
{
    etex::ServerConnection server_connection;
    etex::ClientApi client(server_connection);

    auto asio_thread =
        std::async(std::launch::async, &etex::ServerConnection::connect, &server_connection);

    client.join_service();

    // Add any debug messages here
    // {
    //     etex::common::message msg;
    //     msg.hdr.msg_type = etex::common::message_type::connection_to_user_request;
    //     client.send_message(msg);
    // }

    asio_thread.wait();

    return 0;
}