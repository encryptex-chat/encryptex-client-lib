#include "ServerConnection.hpp"

#include <iostream>

#include "message.hpp"

namespace etex
{
using boost::asio::ip::tcp;

ServerConnection::error_code ServerConnection::connect()
{
    boost::system::error_code ec;

    try
    {
        tcp::resolver resolver(m_io_context);
        tcp::resolver::results_type endpoints =
            resolver.resolve(common::k_server_addr, std::to_string(common::k_server_port), ec);
        if (ec)
        {
            std::cerr << "Error: " << ec.what() << std::endl;
            return ec;
        }

        m_socket = tcp::socket(m_io_context);
        m_timer  = boost::asio::steady_timer(m_io_context);
        (*m_timer).expires_at(std::chrono::steady_clock::time_point::max());

        boost::asio::connect(*m_socket, endpoints, ec);
        if (ec)
        {
            std::cerr << "Error: " << ec.what() << std::endl;
            return ec;
        }

        boost::asio::co_spawn(m_io_context, read_message(), boost::asio::detached);
        boost::asio::co_spawn(m_io_context, send_message(), boost::asio::detached);
        m_io_context.run();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }

    return ec;
}

void ServerConnection::send_message(std::string&& msg)
{
    m_send_msgs_queue.push(std::move(msg));
    (*m_timer).cancel();
}

boost::asio::awaitable<void> ServerConnection::read_message()
{
    std::array<char, 1024> buf;
    try
    {
        while ((*m_socket).is_open())
        {
            const auto len = co_await (*m_socket).async_read_some(boost::asio::buffer(buf),
                                                                  boost::asio::use_awaitable);
            std::cout << "Received " << len << " bytes" << std::endl;
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}

boost::asio::awaitable<void> ServerConnection::send_message()
{
    try
    {
        while ((*m_socket).is_open())
        {
            if (m_send_msgs_queue.empty())
            {
                boost::system::error_code ec;
                co_await (*m_timer).async_wait(boost::asio::deferred);
            }
            else
            {
                co_await (*m_socket).async_write_some(
                    boost::asio::buffer(m_send_msgs_queue.front()), boost::asio::deferred);
                // co_await boost::asio::async_write(*m_socket,
                //                                   boost::asio::buffer(m_send_msgs_queue.front()),
                //                                   boost::asio::deferred);
                m_send_msgs_queue.pop();
            }
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}
}  // namespace etex