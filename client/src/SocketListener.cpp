#include "../include/SocketListener.h"
#include "../include/ConnectionHandler.h"
#include "../include/Frame.h"

SocketListener::SocketListener(ConnectionHandler *handler) : handler(handler){}

void SocketListener::run(){
    while (!shouldTerminate){
        string input;
        handler->getLine(input);
        Frame frame(input);
        handler->protocolProccess(frame);
    }
}