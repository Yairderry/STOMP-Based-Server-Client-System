#pragma once

class Frame;
class ConnectionHandler;

class StompProtocol
{
private:
    ConnectionHandler *handler;
public:
    StompProtocol();
    void proccess(Frame &frame);
    void connected(Frame &frame);
    void error(Frame &frame);
    void receipt(Frame &frame);
    void message(Frame &frame);
    void setHandler(ConnectionHandler *);
    ConnectionHandler &getHandler();

};
