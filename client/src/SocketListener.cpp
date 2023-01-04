#include "../include/SocketListener.h"
#include "../include/ConnectionHandler.h"
#include "../include/Frame.h"
#include <boost/algorithm/string.hpp>


SocketListener::SocketListener(ConnectionHandler *handler) : handler(handler){}

void SocketListener::run(){
    while (!shouldTerminate){
        string input;
        handler->getFrameAscii(input, '\0');
        boost::trim(input);
        Frame frame(input);
        handler->protocolProccess(frame);
    }
}