#pragma once

#include "../include/ConnectionHandler.h"

class SocketListener
{
private:
    ConnectionHandler *handler;

public:
    SocketListener(ConnectionHandler*);
    void run();
};
