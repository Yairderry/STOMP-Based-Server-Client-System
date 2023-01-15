#pragma once

class Frame;

class ConnectionHandler;

class StompProtocol {
private:
    ConnectionHandler *handler;
public:
    StompProtocol();

    bool process(Frame &frame);

    bool connected();

    bool error(Frame &frame);

    bool receipt(Frame &frame);

    bool message(Frame &frame);

    void setHandler(ConnectionHandler *);

};
