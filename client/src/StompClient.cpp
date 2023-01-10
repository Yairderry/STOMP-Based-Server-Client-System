#include "../include/ConnectionHandler.h"
#include "../include/SocketListener.h"
#include "../include/InputManager.h"
#include "../include/StompProtocol.h"

#include <mutex>
#include <thread>

int main(int argc, char *argv[]) {

    std::mutex mutex;
    std::condition_variable cv;
    SocketListener socket_listener_task(mutex, cv);
    InputManager input_manager;

    std::thread socket_listener_thread(&SocketListener::run, &socket_listener_task);
    input_manager.read(socket_listener_task);
    
    socket_listener_thread.join();    

	return 0;
}