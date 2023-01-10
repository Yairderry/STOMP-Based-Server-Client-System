#pragma once

#include <string>
#include <fstream>

using std::string;

class ConnectionHandler;

class InputManager
{
private:
    ConnectionHandler *handler;

public:
    InputManager(ConnectionHandler*);
    void read();
    void login(string &, string &, string &);
    void join(string &);
    void exit(string &);
    void report(string &);
    void summary(string &, string &, string &);
    void logout();
};
