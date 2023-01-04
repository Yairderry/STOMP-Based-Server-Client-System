#pragma once

#include "../include/ConnectionHandler.h"
#include <string>

using std::string;

class InputManager
{
private:
    ConnectionHandler *handler;
    bool shouldTerminate = false;

public:
    InputManager(ConnectionHandler*);
    void run();
    void login(string &, string &, string &);
    void join(string &);
    void exit(string &);
    void report(string &);
    void summary(string &, string &, string &);
    void logout();
};
