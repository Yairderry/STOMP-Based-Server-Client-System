#include "../include/SocketListener.h"
#include "../include/ConnectionHandler.h"
#include "../include/Frame.h"
#include <boost/algorithm/string.hpp>


SocketListener::SocketListener(std::mutex &mutex, std::condition_variable &cv) : handler(nullptr), mutex(mutex),
                                                                                 cv(cv) {}

void SocketListener::run() {

    std::unique_lock<std::mutex> lock(mutex);
    while (handler == nullptr) {
        cv.wait(lock);
        while (!handler->getShouldTerminate()) {
            string input;
            handler->getFrameAscii(input, '\0');
            boost::trim(input);
            Frame frame(input);
            bool proceed = handler->protocolProcess(frame);
            if (!proceed) break;
        }
        handler = nullptr;
        std::cout << "Socket closed." << std::endl;
    }
}

void SocketListener::setHandler(ConnectionHandler *_handler) {
    this->handler = _handler;
    cv.notify_all();
}