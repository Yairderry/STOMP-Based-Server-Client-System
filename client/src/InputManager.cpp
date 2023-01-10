#include "../include/InputManager.h"
#include "../include/ConnectionHandler.h"
#include "../include/Frame.h"
#include "../include/User.h"

InputManager::InputManager(ConnectionHandler *handler) : handler(handler){}

void InputManager::run(){

    string input;
    bool allowInput = true;
    while (!handler->getShouldTerminate() && allowInput){

        // Get next command and parse it
        getline(std::cin,input);
        if (handler->getShouldTerminate() || !allowInput) continue;
        vector<string> args = Frame::split(input, ' ');
        string command = args[0];

        // Create frame and act accordingly
        if (command == "login"){
            if (args.size() != 4){
                std::cout << "Could not connect to server." << std::endl;
                continue;
            }
            login(args[1], args[2], args[3]);
        }
        else if (command == "join")
            join(args[1]);
        else if (command == "exit")
            exit(args[1]);
        else if (command == "report")
            report(args[1]);
        else if (command == "summary") 
            summary(args[1], args[2], args[3]);
        else if (command == "logout"){
            logout();
            allowInput = false;
        }
    } 
        std::cout << "Input manager thread terminated" << std::endl;

}

void InputManager::login(string &host_port, string &username, string &password){
    if (handler->isLoggedIn()){
        std::cout << "The client is already logged in, log out before trying again" << std::endl;
        return;
    }
    try{
        handler->setUser(new User(username, password));
        string version = "1.2";
        ConnectFrame frame(version, host_port, username, password);
        string line = frame.toString();
        handler->sendFrameAscii(line, '\0');
    }
    catch (const std::exception& e){
        std::cout << "Could not connect to server." << std::endl;
    }
}

void InputManager::join(string &game_name){
    User &user = handler->getUser();
    if (!user.getConnected()) return;
    
    string subscriptionId = std::to_string(user.getNextSID());
    string receiptId = "subscribe-" + game_name + "-" + std::to_string(user.getNextRID());
    bool succeeded = user.addSubscription(game_name, std::stoi(subscriptionId));
    if (!succeeded){
        std::cout << "User already subscribed to this topic" << std::endl;
        return;
    }

    SubscribeFrame frame(game_name, subscriptionId, receiptId);
    string line = frame.toString();
    handler->sendFrameAscii(line, '\0');
}

void InputManager::exit(string &game_name){
    User &user = handler->getUser();
    if (!user.getConnected()) return;

    string subscriptionId = std::to_string(user.getSubscriptionId(game_name));
    string receiptId = "unsubscribe-" + game_name + "-" + std::to_string(user.getNextRID());

    bool succeeded = user.removeSubscription(game_name);
    if (!succeeded){
        std::cout << "User is not subscribed to this topic" << std::endl;
        return;
    }

    UnsubscribeFrame frame(subscriptionId, receiptId);
    string line = frame.toString();
    handler->sendFrameAscii(line, '\0');
}

void InputManager::report(string &file_path){
    User &user = handler->getUser();
    if (!user.getConnected()) return;

    names_and_events game = parseEventsFile(file_path);
    
    string destination = game.team_a_name + "_" + game.team_b_name;
    vector<Event> events = game.events;
    sortEvents(events);

    for (Event event : events){
        string body = "user: " + user.getUsername() + "\n" + event.toString();
        string receiptId = "report-" + std::to_string(user.getNextRID());
        SendFrame frame(destination, receiptId, body);
        string line = frame.toString();
        handler->sendFrameAscii(line, '\0');
    }
}

void InputManager::summary(string &game_name, string &reporter_user, string &file_path){
    User &user = handler->getUser();
    if (user.getSubscriptionId(game_name) == -1){
        std::cout << "User is not subscribed to this topic" << std::endl;
        return;
    }
    vector<Event> events = vector<Event>{};
    user.getEvents(events, game_name, reporter_user);

    string output = "";
    vector<string> names = Frame::split(game_name, '_');

    output += names[0] + " vs " + names[1] + '\n' + "Game stats:\n";

    map<string, string> general_stats;
    map<string, string> team_a_stats;
    map<string, string> team_b_stats;
    string game_events = "Game event reports:\n";

    for (unsigned int i = 0; i < events.size(); i++){
        Event &event = events[i];

        game_events +=  event.summarize();

        for (auto pair : event.get_game_updates())
            general_stats[pair.first] = pair.second;
        for (auto pair : event.get_team_a_updates())
            team_a_stats[pair.first] = pair.second;
        for (auto pair : event.get_team_b_updates())
            team_b_stats[pair.first] = pair.second;    
    }

    output += "General stats:\n";
    for (auto pair : general_stats)
        output += pair.first + ": " + pair.second + '\n';
    output += names[0] + " stats:\n";
    for (auto pair : team_a_stats)
        output += pair.first + ": " + pair.second + '\n';
    output += names[1] + " stats:\n";
    for (auto pair : team_b_stats)
        output += pair.first + ": " + pair.second + '\n';

    output += game_events;

    std::ofstream outputFile(file_path, std::ofstream::trunc);
    outputFile << output << std::endl;
    outputFile.close();
}

void InputManager::logout(){
    User &user = handler->getUser();
    if (!user.getConnected()) return;

    string receiptId = "disconnect-" + std::to_string(user.getNextRID());
    DisconnectFrame frame(receiptId);
    string line = frame.toString();
    handler->sendFrameAscii(line, '\0');
    }

