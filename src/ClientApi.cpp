#include "ClientApi.hpp"

#include <spdlog/spdlog.h>

#include <iostream>
#include <magic_enum/magic_enum.hpp>

#include "ServerConnection.hpp"

namespace etex
{
ClientApi::ClientApi(ServerConnection& server_connection) : m_server_connection(server_connection)
{
    m_server_connection.subscribe_to_receive([this](common::message msg)
                                             { this->receive_msg(msg); });
}

void ClientApi::join_service()
{
    common::message msg;
    msg.hdr.msg_type = common::message_type::connection_to_server_request;
    m_server_connection.send_message(std::move(msg));
}

void ClientApi::send_message(common::message&& msg)
{
    m_server_connection.send_message(std::move(msg));
}

void ClientApi::receive_msg(common::message msg)
{
    spdlog::info("ClientApi: received message (Type: {})", magic_enum::enum_name(msg.hdr.msg_type));
}
}  // namespace etex