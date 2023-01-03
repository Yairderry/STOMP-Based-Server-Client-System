CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system -lpthread

all: StompWCIClient

StompWCIClient: bin/ConnectionHandler.o bin/StompClient.o bin/event.o bin/Frame.o bin/InputManager.o bin/SocketListener.o
	g++ -o bin/StompWCIClient bin/ConnectionHandler.o bin/StompClient.o bin/Frame.o bin/InputManager.o bin/SocketListener.o $(LDFLAGS)

bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/event.o: src/event.cpp
	g++ $(CFLAGS) -o bin/event.o src/event.cpp

bin/StompClient.o: src/StompClient.cpp
	g++ $(CFLAGS) -o bin/StompClient.o src/StompClient.cpp

bin/Frame.o: src/Frame.cpp
	g++ $(CFLAGS) -o bin/Frame.o src/Frame.cpp

bin/SocketListener.o: src/SocketListener.cpp
	g++ $(CFLAGS) -o bin/SocketListener.o src/SocketListener.cpp

bin/InputManager.o: src/InputManager.cpp
	g++ $(CFLAGS) -o bin/InputManager.o src/InputManager.cpp

.PHONY: clean
clean:
	rm -f bin/*
	