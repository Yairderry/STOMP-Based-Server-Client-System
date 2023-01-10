#pragma once

#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include "User.h"
#include "StompProtocol.h"

using boost::asio::ip::tcp;
using std::string;

class Frame;
// class StompProtocol;
// class User;

class ConnectionHandler {
private:
	const std::string host_;
	const short port_;
	boost::asio::io_service io_service_;   // Provides core I/O functionality
	tcp::socket socket_;
	User user;
	StompProtocol protocol;
	bool shouldTerminate = false;

public:
	ConnectionHandler(std::string host, short port);

	virtual ~ConnectionHandler(); // destructor
	// ConnectionHandler(const ConnectionHandler &); // copy constructor
    // ConnectionHandler(ConnectionHandler &&) noexcept ; // move constructor
    // ConnectionHandler &operator=(const ConnectionHandler &); // copy assignment operator
    // ConnectionHandler &operator=(ConnectionHandler &&) noexcept ; // move assignment operator

	bool getShouldTerminate();
	void terminate();

	void setUser(User &);
	User &getUser();
	bool isLoggedIn();

	void setProtocol(StompProtocol &);
	StompProtocol &getProtocol();

	// Connect to the remote machine
	bool connect();

	// Read a fixed number of bytes from the server - blocking.
	// Returns false in case the connection is closed before bytesToRead bytes can be read.
	bool getBytes(char bytes[], unsigned int bytesToRead);

	// Send a fixed number of bytes from the client - blocking.
	// Returns false in case the connection is closed before all the data is sent.
	bool sendBytes(const char bytes[], int bytesToWrite);

	// Read an ascii line from the server
	// Returns false in case connection closed before a newline can be read.
	bool getLine(std::string &line);

	// Send an ascii line from the server
	// Returns false in case connection closed before all the data is sent.
	bool sendLine(std::string &line);

	// Get Ascii data from the server until the delimiter character
	// Returns false in case connection closed before null can be read.
	bool getFrameAscii(std::string &frame, char delimiter);

	// Send a message to the remote host.
	// Returns false in case connection is closed before all the data is sent.
	bool sendFrameAscii(const std::string &frame, char delimiter);

	// Close down the connection properly.
	void close();

	// Process by STOMP protocol
	void protocolProccess(Frame &frame);

}; //class ConnectionHandler
