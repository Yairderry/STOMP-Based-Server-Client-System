#include "../include/StompProtocol.h"
#include "../include/Frame.h"
#include "../include/ConnectionHandler.h"
#include "../include/User.h"

StompProtocol::StompProtocol(){}

void StompProtocol::proccess(Frame &frame){
    string command = frame.getCommand();
    if (command == "CONNECTED")
        connected(frame);
    else if (command == "ERROR")
        error(frame);
    else if (command == "RECEIPT")
        receipt(frame);
    else if (command == "MESSAGE")
        message(frame);
}

void StompProtocol::connected(Frame &frame){
    User &user = handler->getUser();
    user.toggleConnected();
    std::cout << "Login Successful" << std::endl;
}

void StompProtocol::error(Frame &frame){
    std::cout << frame.toString() << std::endl;

    handler->setUser(nullptr);
    handler->terminate();    
}

void StompProtocol::receipt(Frame &frame){
    User &user = handler->getUser();
    string receiptId = frame.getHeader("receipt-id");

    vector<string> receipt_args = Frame::split(receiptId, '-');

    string action = receipt_args[0];

    if (action == "disconnect"){
        user.toggleConnected();
        handler->setUser(nullptr);
        handler->terminate();
        std::cout << "Goodbye" << std::endl;
    }
}

void StompProtocol::message(Frame &frame){
    User &user = handler->getUser();
    string game_name = user.getChannelById(std::stoi(frame.getHeader("subscription")));
    vector<string> body_lines = Frame::split(frame.getBody(), '\n');
    string first_line = body_lines[0];
    string reporter_user = first_line.substr(first_line.find(":") + 2, first_line.size()-1);

    Event reported_event(frame.getBody());

    user.addEvent(reported_event, game_name, reporter_user);
}

void StompProtocol::setHandler(ConnectionHandler *handler){
    this->handler = handler;
}

ConnectionHandler &StompProtocol::getHandler(){
    return *handler;
}