#pragma once

#include "../include/ConnectionHandler.h"

class SocketListener
{
private:
    SocketListener *handler;

public:
    SocketListener(ConnectionHandler *);
    void run();
};
