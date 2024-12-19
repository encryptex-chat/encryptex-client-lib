#pragma once
#include <boost/asio.hpp>
#include <condition_variable>
#include <mutex>
#include <queue>

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

    private:
    /**
     * Here may be interruction with some UI or terminal thread
     */
    void receive_msg(common::message msg);

    ServerConnection& m_server_connection;
};
}  // namespace etex