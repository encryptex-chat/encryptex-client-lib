#pragma once

namespace etex
{
class ServerConnection;
class ClientApi
{
    public:
    ClientApi(ServerConnection& server_connection) : m_server_connection(server_connection) {}
    void join_service();

    private:
    ServerConnection& m_server_connection;
};
}  // namespace etex