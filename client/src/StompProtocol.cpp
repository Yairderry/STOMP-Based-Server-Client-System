#include "../include/StompProtocol.h"
#include "../include/Frame.h"
#include "../include/ConnectionHandler.h"

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

}

void StompProtocol::error(Frame &frame){
    
}

void StompProtocol::receipt(Frame &frame){
    
}

void StompProtocol::message(Frame &frame){
    
}

void StompProtocol::setHandler(ConnectionHandler *handler){
    this->handler = handler;
}

ConnectionHandler &StompProtocol::getHandler(){
    return *handler;
}