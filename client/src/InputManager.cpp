#include "../include/InputManager.h"
#include "../include/Frame.h"
#include "../include/Event.h"
#include <string>

using std::string;

InputManager::InputManager(ConnectionHandler *handler) : handler(handler) {}

void InputManager::run(){
    while (!shouldTerminate){

        // Get next command and parse it
        string input;
        getline(std::cin,input);
        vector<string> args = Frame::split(input, ' ');
        string command = args[0];

        // Create frame and act accordingly
        if (command == "login")
            login(args[1], args[2], args[3]);
        else if (command == "join")
            join(args[1]);
        else if (command == "exit")
            exit(args[1]);
        else if (command == "report")
            report(args[1]);
        // else if (command == "summary") 
        //     summary(args[1], args[2], args[3]);
        // else if (command == "logout")
        //     logout();
    } 

}

void InputManager::login(string &host_port, string &username, string &password){
    handler->setUser(new User(username, password));
    string version = "1.2";
    ConnectFrame frame(version, host_port, username, password);
    string line = frame.toString();
    handler->sendLine(line);
}

void InputManager::join(string &game_name){
    User &user = handler->getUser();
    string subscriptionId = std::to_string(user.getNextSID());
    string receiptId = std::to_string(user.getNextRID());
    SubscribeFrame frame(game_name, subscriptionId, receiptId);
    string line = frame.toString();
    handler->sendLine(line);
}

void InputManager::exit(string &game_name){
    User &user = handler->getUser();
    string subscriptionId = std::to_string(user.getSubscriptionId(game_name));
    string receiptId = std::to_string(user.getNextRID());
    UnsubscribeFrame frame(subscriptionId, receiptId);
    string line = frame.toString();
    handler->sendLine(line);
}

void InputManager::report(string &file_path){
    User &user = handler->getUser();
    names_and_events game = parseEventsFile(file_path);
    string destination; game.team_a_name + "_" + game.team_b_name;
    vector<Event> events = game.events;
    std::sort(events.begin(), events.end(), [](const Event & a, const Event & b) -> bool{ return a.get_time() > b.get_time(); });
    // user.addGame(destination, events);
    
    for (Event event : events){
        string body = "user: " + user.getUsername() + "/n" + event.toString();
        string receiptId = std::to_string(user.getNextRID());
        SendFrame frame(destination, receiptId, body);
        string line = frame.toString();
        std::cout << line << std::endl;
        handler->sendLine(line);
    }
}

