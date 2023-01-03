#include "../include/ConnectionHandler.h"
#include "../include/SocketListener.h"
#include "../include/InputManager.h"
#include <mutex>
#include <thread>

int main(int argc, char *argv[]) {

	ConnectionHandler *handler = new ConnectionHandler(argv[1], (short) argv[2]);

    SocketListener socketListenerTask (handler);
    InputManager inputManagerTask(handler);

	std::thread socketListenerThread(&SocketListener::run, &socketListenerTask);
	std::thread inputManagerThread(&InputManager::run, &inputManagerTask);

    socketListenerThread.join();
    inputManagerThread.join();

	return 0;
}