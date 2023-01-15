#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <thread>

using std::string;

class ConnectionHandler;

class SocketListener;

class InputManager {
private:
    ConnectionHandler *handler;

public:
    InputManager();

    void read(SocketListener &);

    ConnectionHandler *login(string &, string &, string &);

    void join(string &);

    void exit(string &);

    void report(string &);

    void summary(string &, string &, string &);

    void logout();
};
