#pragma once

#include <boost/asio.hpp>
#include <optional>
// #include <queue>

#include "common.hpp"

namespace etex
{
class ServerConnection
{
    public:
    void connect();
    void send_message(common::message&& msg);
    void stop();
    void subscribe_to_receive(std::function<void(common::message)> callback_func)
    {
        m_received_msg_notifier = callback_func;
    }

    private:
    boost::asio::awaitable<void> read_message();
    boost::asio::awaitable<void> send_message();

    boost::asio::io_context m_io_context;
    std::optional<boost::asio::ip::tcp::socket> m_socket;
    // std::queue<common::message> m_send_msgs_queue;
    common::message m_received_msg_buffer;
    std::function<void(common::message)> m_received_msg_notifier;
};
}  // namespace etex