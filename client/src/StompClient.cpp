#include "../include/ConnectionHandler.h"
#include "../include/SocketListener.h"
#include "../include/InputManager.h"
#include "../include/StompProtocol.h"

#include <mutex>
#include <thread>

int main(int argc, char *argv[]) {

	if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }
	std::string host = argv[1];
    short port = atoi(argv[2]);

	ConnectionHandler *handler = new ConnectionHandler(host, port);
    StompProtocol *protocol = new StompProtocol();

    handler->setProtocol(protocol);
    protocol->setHandler(handler);


	if (!handler->connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }

    SocketListener socket_listener(handler);
    InputManager input_manager_task(handler);

    std::thread input_manager(&InputManager::run, &input_manager_task);
    socket_listener.listen();
    
    input_manager.join();    

	return 0;
}