#include "ClientApi.hpp"

#include <spdlog/spdlog.h>

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
void ClientApi::request_connect_to_user(const uint64_t user_id)
{
    common::message msg;
    msg.hdr.msg_type = common::message_type::connection_to_user_request;
    msg.hdr.src_id   = m_my_id;
    msg.hdr.dst_id   = user_id;
    m_server_connection.send_message(std::move(msg));
}

void ClientApi::send_message_to_user(const uint64_t user_id, const std::string&& text_msg)
{
    common::message msg;
    msg.hdr.msg_type = common::message_type::data_transfer;
    msg.hdr.src_id   = m_my_id;
    msg.hdr.dst_id   = user_id;
    std::copy(text_msg.begin(), text_msg.end(), msg.data.begin());
    m_server_connection.send_message(std::move(msg));
}

void ClientApi::receive_msg(common::message msg)
{
    // spdlog::info("ClientApi: received message (Type: {})",
    // magic_enum::enum_name(msg.hdr.msg_type));
    if (msg.hdr.msg_type == common::message_type::connection_to_server_response)
    {
        m_my_id = msg.hdr.src_id;
    }
    m_received_msg_processor(std::move(msg));
}
}  // namespace etex