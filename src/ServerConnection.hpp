#pragma once

#include <boost/asio.hpp>
#include <optional>
#include <queue>

#include "common.hpp"

namespace etex
{
class ServerConnection
{
    public:
    using error_code = boost::system::error_code;

    error_code connect();
    void send_message(common::message&& msg);

    private:
    boost::asio::awaitable<void> read_message();
    boost::asio::awaitable<void> send_message();

    boost::asio::io_context m_io_context;
    std::optional<boost::asio::ip::tcp::socket> m_socket;
    std::optional<boost::asio::steady_timer> m_timer;
    std::queue<common::message> m_send_msgs_queue;
    common::message m_received_msg_buffer;
};
}  // namespace etex