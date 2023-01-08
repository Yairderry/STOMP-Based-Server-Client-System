#include "../include/SocketListener.h"
#include "../include/ConnectionHandler.h"
#include "../include/Frame.h"
#include <boost/algorithm/string.hpp>


SocketListener::SocketListener(ConnectionHandler *handler) : handler(handler){}

void SocketListener::run(){
    while (!handler->getShouldTerminate()){
        string input;
        handler->getFrameAscii(input, '\0');
        boost::trim(input);
        Frame frame(input);
        std::cout << "--------------Received Frame---------------------" << std::endl;
        std::cout << frame.toString() << std::endl;
        std::cout << "--------------End Received Frame-----------------" << std::endl;

        handler->protocolProccess(frame);
    }
}