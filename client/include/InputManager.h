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
    void login(string &host_port, string &username, string &password);
    void join(string &game_name);
    void exit(string &game_name);
    void report(string &file_path);
    void summary(string &game_name, string &user, string &file_path);
    void logout();
};
