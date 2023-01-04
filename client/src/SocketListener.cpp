#include "../include/SocketListener.h"
#include "../include/ConnectionHandler.h"
#include "../include/Frame.h"

SocketListener::SocketListener(ConnectionHandler *handler) : handler(handler){}

void SocketListener::run(){
    while (!shouldTerminate){
        string input;
        handler->getFrameAscii(input, '\0');
        Frame frame(input);
        std::cout << "//////////////////" << std::endl;
        std::cout << frame.toString() << std::endl;
        std::cout << "//////////////////" << std::endl;
        handler->protocolProccess(frame);
    }
}