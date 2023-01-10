#pragma once

#include <string>

using std::string;

class ConnectionHandler;

class SocketListener
{
private:
    ConnectionHandler *handler;

public:
    SocketListener();
    void run();
    void setHandler(ConnectionHandler *);
};
