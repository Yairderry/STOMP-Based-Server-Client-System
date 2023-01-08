CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system -lpthread

all: StompWCIClient

StompWCIClient: bin/ConnectionHandler.o bin/StompClient.o bin/Event.o bin/Frame.o bin/InputManager.o bin/SocketListener.o bin/User.o bin/StompProtocol.o bin/Forum.o
	g++ -o bin/StompWCIClient bin/ConnectionHandler.o bin/StompClient.o bin/Event.o bin/Frame.o bin/InputManager.o bin/SocketListener.o bin/User.o bin/StompProtocol.o bin/Forum.o $(LDFLAGS)

bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/Event.o: src/Event.cpp
	g++ $(CFLAGS) -o bin/Event.o src/Event.cpp

bin/StompClient.o: src/StompClient.cpp
	g++ $(CFLAGS) -o bin/StompClient.o src/StompClient.cpp

bin/Frame.o: src/Frame.cpp
	g++ $(CFLAGS) -o bin/Frame.o src/Frame.cpp

bin/SocketListener.o: src/SocketListener.cpp
	g++ $(CFLAGS) -o bin/SocketListener.o src/SocketListener.cpp

bin/InputManager.o: src/InputManager.cpp
	g++ $(CFLAGS) -o bin/InputManager.o src/InputManager.cpp

bin/User.o: src/User.cpp
	g++ $(CFLAGS) -o bin/User.o src/User.cpp

bin/StompProtocol.o: src/StompProtocol.cpp
	g++ $(CFLAGS) -o bin/StompProtocol.o src/StompProtocol.cpp

bin/Forum.o: src/Forum.cpp
	g++ $(CFLAGS) -o bin/Forum.o src/Forum.cpp

.PHONY: clean
clean:
	rm -f bin/*
	