#pragma once

#include <string>

using std::string;

class ConnectionHandler;

class SocketListener
{
private:
    ConnectionHandler *handler;
    bool shouldTerminate = false;

public:
    SocketListener(ConnectionHandler*);
    void run();
};
