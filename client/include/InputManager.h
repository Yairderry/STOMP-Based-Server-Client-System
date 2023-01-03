#pragma once

#include "../include/ConnectionHandler.h"

class InputManager
{
private:
    ConnectionHandler *handler;

public:
    InputManager(ConnectionHandler *);
    void run();
};
