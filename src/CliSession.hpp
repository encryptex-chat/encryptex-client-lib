#pragma once

#include "common.hpp"

namespace etex
{
class ClientApi;
class CliSession
{
    public:
    CliSession(ClientApi& client_api);
    void run();

    void process_received_msg(const common::message&& msg);
    void print_received_msg(const common::message& msg);

    private:
    void print_help() const;

    ClientApi& m_client_api;
    bool m_running{false};
    bool m_process_connection_request{false};
    /**
     * TODO: need to implement thread safe queue for received messages
     * if there will be simultaneous requests from multiple users
     */
    uint64_t m_requested_user_id;
};
}  // namespace etex