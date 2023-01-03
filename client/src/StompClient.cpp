#include "../include/ConnectionHandler.h"
#include "../include/SocketListener.h"
#include "../include/InputManager.h"
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

	if (!handler->connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }

    SocketListener socketListenerTask(handler);
    InputManager inputManagerTask(handler);

	std::thread socketListenerThread(&SocketListener::run, &socketListenerTask);
	std::thread inputManagerThread(&InputManager::run, &inputManagerTask);

    socketListenerThread.join();
    inputManagerThread.join();

	return 0;
}