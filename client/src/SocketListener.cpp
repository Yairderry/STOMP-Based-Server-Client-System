#include "../include/SocketListener.h"
#include "../include/ConnectionHandler.h"
#include "../include/Frame.h"
#include <boost/algorithm/string.hpp>


SocketListener::SocketListener(std::mutex &mutex, std::condition_variable &cv) : handler(nullptr), mutex(mutex), cv(cv){}

void SocketListener::run(){

    std::unique_lock<std::mutex> lock(mutex);
    while (handler == nullptr){
        cv.wait(lock);
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
    handler = nullptr;
    std::cout << "Socket closed." << std::endl;
    }
}

void SocketListener::setHandler(ConnectionHandler *handler){
    this->handler = handler;
    cv.notify_all();
}