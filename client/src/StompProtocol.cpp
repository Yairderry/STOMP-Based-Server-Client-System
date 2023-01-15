#include "../include/StompProtocol.h"
#include "../include/Frame.h"
#include "../include/ConnectionHandler.h"

StompProtocol::StompProtocol() : handler(nullptr) {}

bool StompProtocol::process(Frame &frame) {
    string command = frame.getCommand();
    if (command == "CONNECTED")
        return connected();
    else if (command == "ERROR")
        return error(frame);
    else if (command == "RECEIPT")
        return receipt(frame);
    else if (command == "MESSAGE")
        return message(frame);
    return false;
}

bool StompProtocol::connected() {
    User &user = handler->getUser();
    user.toggleConnected();
    std::cout << "Login Successful." << std::endl;
    return true;
}

bool StompProtocol::error(Frame &frame) {
    std::cout << frame.toString() << std::endl;

    handler->terminate();
    if (handler) delete handler;

    return false;

}

bool StompProtocol::receipt(Frame &frame) {
    User &user = handler->getUser();
    string receiptId = frame.getHeader("receipt-id");

    vector<string> receipt_args = Frame::split(receiptId, '-');

    string action = receipt_args[0];

    if (action == "disconnect") {
        user.toggleConnected();
        handler->terminate();
        if (handler) delete handler;
        return false;
    } else if (action == "subscribe")
        std::cout << "Joined channel " << receipt_args[1] << "." << std::endl;
    else if (action == "unsubscribe")
        std::cout << "Exited channel " << receipt_args[1] << "." << std::endl;
    return true;
}

bool StompProtocol::message(Frame &frame) {
    User &user = handler->getUser();
    string game_name = user.getChannelById(std::stoi(frame.getHeader("subscription")));
    vector<string> body_lines = Frame::split(frame.getBody(), '\n');
    string first_line = body_lines[0];
    string reporter_user = first_line.substr(first_line.find(':') + 2, first_line.size() - 1);

    Event reported_event(frame.getBody());

    user.addEvent(reported_event, game_name, reporter_user);

    return true;
}

void StompProtocol::setHandler(ConnectionHandler *_handler) {
    this->handler = _handler;
}