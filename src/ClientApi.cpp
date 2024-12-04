#include "ClientApi.hpp"

#include "ServerConnection.hpp"

namespace etex
{
void ClientApi::join_service()
{
    common::message msg;
    msg.hdr.msg_type = common::message_type::connection_to_server_request;
    m_server_connection.send_message(std::move(msg));
}
}  // namespace etex