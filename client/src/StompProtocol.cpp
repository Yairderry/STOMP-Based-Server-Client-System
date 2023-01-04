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
    else if (command == "RECEIPT"){
        receipt(frame);
    }
    else if (command == "MESSAGE")
        message(frame);
}

void StompProtocol::connected(Frame &frame){
    User &user = handler->getUser();
    user.toggleConnected();
    std::cout << "Login Successful" << std::endl;
}

void StompProtocol::error(Frame &frame){
    std::cout << "error" << std::endl;
    
}

void StompProtocol::receipt(Frame &frame){
    std::cout << "receipt" << std::endl;
}

void StompProtocol::message(Frame &frame){
    std::cout << "message" << std::endl;
}

void StompProtocol::setHandler(ConnectionHandler *handler){
    this->handler = handler;
}

ConnectionHandler &StompProtocol::getHandler(){
    return *handler;
}