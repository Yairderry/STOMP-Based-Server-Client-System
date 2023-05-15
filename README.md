# STOMP Based Server Client System
This repository contains the implementation of a STOMP (Streaming Text Oriented Messaging Protocol) server and client in SPL (Systems Programming Language) course at the university. The aim of this project is to provide a simple messaging system that allows clients to communicate with each other through the server.
The server is fully implemented in Java and uses the TCP and Reactor design patterns.

## Features
- Support for sending and receiving messages between clients through the server.
- Support for multiple clients connecting to the server at the same time.
- Implementation of basic STOMP protocol commands: CONNECT, SEND, SUBSCRIBE, UNSUBSCRIBE, and DISCONNECT.

## Requirements
- A C++ compiler that supports C++11.
- Boost library for networking and threading.
- Java 8 or higher.


## Getting Started 
- Clone the repository to your local machine.

### Server Start
- Access folder: `cd server`
- Build using: `mvn compile`
- Run the server (choose one design pattern):
  - TPC pattern: `mvn exec:java -Dexec.mainClass="bgu.spl.net.impl.stomp.StompServer" -Dexec.args="<port> tpc"`
  - Reactor pattern: `mvn exec:java -Dexec.mainClass="bgu.spl.net.impl.stomp.StompServer" -Dexec.args="<port> reactor`

### Client Start
- Access folder: `cd client`
- Build using: `make`
- Run the client: `bin/StompWCIClient`

### Client Commands
- Login: `login {host:port} {username} {password}`
- Subscribe: `join {channel_name}`
- Unsubscribe: `exit {channel_name}`
- Send: `report {file_path}`
- Summary: `summary {channel_name} {user} {file}`
- Logout: `logout`

## Example

- Command: `login 127.0.0.1:7777 meni films`
  - Frame sent: ![image](https://user-images.githubusercontent.com/109943831/218320972-59d844b0-17d2-4d90-9c1e-ee258132cb18.png)
  - Frame received: ![image](https://user-images.githubusercontent.com/109943831/218320992-22604065-38d8-4990-98ad-adaf42c35017.png)
- Command: `join germany_spain`
  - Frame sent: ![image](https://user-images.githubusercontent.com/109943831/218321027-13085d67-0206-45db-9104-aefcc96976f7.png)
  - Frame received: ![image](https://user-images.githubusercontent.com/109943831/218321051-10a23235-c44c-4870-af4e-6b792b56326c.png)
- Command: `exit germany_spain`
  - Frame sent: ![image](https://user-images.githubusercontent.com/109943831/218321097-9e449853-9afe-4bc3-86e1-32239026457b.png)
  - Frame received: ![image](https://user-images.githubusercontent.com/109943831/218321122-40eb9524-7fdd-49ac-ba20-d35ee6a8f880.png)
- Command: `logout`
  - Frame sent: ![image](https://user-images.githubusercontent.com/109943831/218321304-e7d1d1be-9ecf-4a8e-b507-9a294be75c48.png)
  - Frame received: ![image](https://user-images.githubusercontent.com/109943831/218321315-a209d321-d0ca-4233-990f-246ae02e940d.png)

Try it yourself:
- Command: `login 127.0.0.1:7777 user pass`
- Command: `join Germany_Japan`
- Command: `report ./data/events1.json`
- Command: `summary Germany_Japan user ./data/test`
- Command: `exit Germany_Japan`
- Command: `logout`
