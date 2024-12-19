#include "ServerConnection.hpp"

#include <iostream>

namespace etex
{
using boost::asio::ip::tcp;

void ServerConnection::connect()
{
    try
    {
        tcp::resolver resolver(m_io_context);
        tcp::resolver::results_type endpoints =
            resolver.resolve(common::k_server_addr, std::to_string(common::k_server_port));

        m_socket = tcp::socket(m_io_context);

        boost::asio::connect(*m_socket, endpoints);

        boost::asio::co_spawn(m_io_context, read_message(), boost::asio::detached);
        m_io_context.run();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}

void ServerConnection::send_message(common::message&& msg)
{
    boost::asio::co_spawn(
        m_io_context,
        [&]() -> boost::asio::awaitable<void>
        {
            try
            {
                if ((*m_socket).is_open())
                {
                    co_await boost::asio::async_write(
                        *m_socket, boost::asio::buffer(&msg, common::k_message_size),
                        boost::asio::use_awaitable);
                }
            }
            catch (const std::exception& ex)
            {
                std::cerr << ex.what() << std::endl;
            }
        },
        boost::asio::detached);
}

boost::asio::awaitable<void> ServerConnection::read_message()
{
    try
    {
        while ((*m_socket).is_open())
        {
            co_await boost::asio::async_read(
                *m_socket, boost::asio::buffer(&m_received_msg_buffer, common::k_message_size),
                boost::asio::use_awaitable);
            m_received_msg_notifier(m_received_msg_buffer);
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}
}  // namespace etex