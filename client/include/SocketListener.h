#pragma once

#include <string>
#include <mutex>
#include <condition_variable>

using std::string;

class ConnectionHandler;

class SocketListener
{
private:
    ConnectionHandler *handler;
    std::mutex & mutex;
    std::condition_variable & cv;

public:
    SocketListener(std::mutex&, std::condition_variable&);
    void run();
    void setHandler(ConnectionHandler *);
};
