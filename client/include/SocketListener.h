#pragma once

#include <string>

using std::string;

class ConnectionHandler;

class SocketListener
{
private:
    ConnectionHandler *handler;

public:
    SocketListener(ConnectionHandler*);
    void listen();
};
