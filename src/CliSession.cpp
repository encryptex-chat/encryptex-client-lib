#include "CliSession.hpp"

#include <algorithm>
#include <iostream>
#include <magic_enum/magic_enum.hpp>
#include <sstream>
#include <string>

#include "ClientApi.hpp"

namespace etex
{

CliSession::CliSession(ClientApi& client_api) : m_client_api(client_api)
{
    m_client_api.set_received_msg_processor([this](common::message&& msg)
                                            { this->process_received_msg(std::move(msg)); });
}

void CliSession::run()
{
    try
    {
        m_running = true;
        std::string input;
        std::cout << "Terminal session is active (enter 'q' to quit)\n";
        print_help();
        while (m_running)
        {
            std::cout << "> ";
            std::getline(std::cin, input);

            if (m_process_connection_request)
            {
                bool decided{false};
                bool response;
                while (!decided)
                {
                    if (input == "y")
                    {
                        response = true;
                        decided  = true;
                    }
                    else if (input == "n")
                    {
                        response = false;
                        decided  = true;
                    }
                    else
                    {
                        std::cout << "Unknown answer. Please write 'y' or 'n'"
                                  << "\nAccept connection?(y/n) ";
                        std::getline(std::cin, input);
                    }
                }
                m_client_api.respond_to_connection_request(m_requested_user_id, response);
                m_process_connection_request = false;
                continue;
            }

            if (input == "q")
            {
                std::cout << "Session terminated" << std::endl;
                break;
            }

            std::istringstream stream(input);
            std::string str;

            stream >> str;

            if (str.empty())
                continue;

            if (str == "connect")
            {
                stream >> str;
                if (!std::ranges::all_of(str.begin(), str.end(), ::isdigit))
                {
                    std::cout << "Invalid user ID" << std::endl;
                    continue;
                }
                m_client_api.request_connect_to_user(std::stoul(str));
                // std::cout << "Request to connect to ID " << str << " sent" << std::endl;
            }
            else if (str == "send")
            {
                stream >> str;
                if (!std::ranges::all_of(str.begin(), str.end(), ::isdigit))
                {
                    std::cout << "Invalid user ID" << std::endl;
                    continue;
                }
                const uint64_t user_id = std::stoul(str);

                std::string message;
                std::getline(stream, message);
                if (!message.empty() && message[0] == ' ')
                {
                    message.erase(0, 1);
                }
                m_client_api.send_message_to_user(user_id, std::move(message));
            }
            else if (str == "my_id")
            {
                std::cout << "Current ID: " << m_client_api.get_my_id() << std::endl;
            }
            else if (str == "help")
            {
                print_help();
            }
            else
            {
                std::cout << "Unknown command" << std::endl;
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void CliSession::process_received_msg(const common::message&& msg)
{
    std::cout << '\a';  // sound notification
    switch (msg.hdr.msg_type)
    {
        case common::message_type::user_to_user_request:
            m_process_connection_request = true;
            m_requested_user_id          = msg.hdr.dst_id;
            std::cout << "\nReceived connection request from user #" << m_requested_user_id
                      << "\nAccept connection?(y/n) ";
            std::cout.flush();
            break;

        default: print_received_msg(msg);
    }
}

void CliSession::print_received_msg(const common::message& msg)
{
    std::cout << "\nReceived message:\n"
              << "Type: " << magic_enum::enum_name(msg.hdr.msg_type) << '\n'
              << "Source ID: " << msg.hdr.src_id << '\n';
    if (msg.hdr.msg_type == common::message_type::data_transfer)
    {
        std::cout << "Data: " << msg.data.data() << '\n';
    }
    std::cout << "> ";
    std::cout.flush();
}

void CliSession::print_help() const
{
    std::cout << "Available actions:\n"
              << "my_id - get my ID\n"
              << "connect <user_id> - request connect to user\n"
              << "send <user_id> <message> - send text message to user\n"
              << "q - terminate session\n";
}
}  // namespace etex