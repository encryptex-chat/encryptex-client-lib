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

    void print_received_msg(const common::message&& msg);

    private:
    void print_help() const;

    ClientApi& m_client_api;
    bool m_running{false};
};
}  // namespace etex