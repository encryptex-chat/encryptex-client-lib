#pragma once

#include <functional>
#include <string>

#include "common.hpp"

namespace etex
{
class ServerConnection;
class ClientApi
{
    public:
    ClientApi(ServerConnection& server_connection);

    void join_service();
    void send_message(common::message&& msg);
    void request_connect_to_user(const uint64_t user_id);
    void send_message_to_user(const uint64_t user_id, const std::string&& text_msg);
    [[nodiscard]] uint64_t get_my_id() const { return m_my_id; }

    void set_received_msg_processor(std::function<void(common::message&&)>&& msg_processor)
    {
        m_received_msg_processor = msg_processor;
    }

    private:
    /**
     * Here may be interruction with some UI or terminal thread
     */
    void receive_msg(common::message msg);

    ServerConnection& m_server_connection;
    uint64_t m_my_id{0};
    std::function<void(common::message&&)> m_received_msg_processor;
};
}  // namespace etex