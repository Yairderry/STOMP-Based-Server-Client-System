#include "../include/ConnectionHandler.h"
#include "../include/SocketListener.h"
#include "../include/InputManager.h"
#include "../include/StompProtocol.h"

#include <mutex>
#include <thread>

int main(int argc, char *argv[]) {

	// if (argc < 3) {
    //     std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
    //     return -1;
    // }
	// std::string host = argv[1];
    // short port = atoi(argv[2]);

	// ConnectionHandler *handler = new ConnectionHandler(host, port);
    // StompProtocol *protocol = new StompProtocol();

    // handler->setProtocol(protocol);
    // protocol->setHandler(handler);


	// if (!handler->connect()) {
    //     std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
    //     return 1;
    // }

    SocketListener socket_listener_task;
    InputManager input_manager;

    std::thread socket_listener_thread(&SocketListener::run, &socket_listener_task);
    input_manager.read(socket_listener_task);
    
    socket_listener_thread.join();    

	return 0;
}