#include "../include/InputManager.h"
#include "../include/ConnectionHandler.h"
#include "../include/Frame.h"
#include "../include/User.h"

InputManager::InputManager(ConnectionHandler *handler) : handler(handler) {}

void InputManager::run(){
    while (!handler->getShouldTerminate()){

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
        else if (command == "summary") 
            summary(args[1], args[2], args[3]);
        else if (command == "logout")
            logout();
    } 
        std::cout << "input manager terminated" << std::endl;

}

void InputManager::login(string &host_port, string &username, string &password){
    if (handler->isLoggedIn()){
        std::cout << "The client is already logged in, log out before trying again" << std::endl;
        return;
    }
    handler->setUser(new User(username, password));
    string version = "1.2";
    ConnectFrame frame(version, host_port, username, password);
    string line = frame.toString();
    handler->sendLine(line);
}

void InputManager::join(string &game_name){
    User &user = handler->getUser();
    if (!user.getConnected()) return;
    
    string subscriptionId = std::to_string(user.getNextSID());
    string receiptId = "subscribe-" + std::to_string(user.getNextRID());
    user.addSubscription(game_name, std::stoi(subscriptionId));

    SubscribeFrame frame(game_name, subscriptionId, receiptId);
    string line = frame.toString();
    handler->sendLine(line);
}

void InputManager::exit(string &game_name){
    User &user = handler->getUser();
    if (!user.getConnected()) return;

    string subscriptionId = std::to_string(user.getSubscriptionId(game_name));
    string receiptId = "unsubscribe-" + std::to_string(user.getNextRID());

    user.removeSubscription(game_name);

    UnsubscribeFrame frame(subscriptionId, receiptId);
    string line = frame.toString();
    handler->sendLine(line);
}

void InputManager::report(string &file_path){
    User &user = handler->getUser();
    if (!user.getConnected()) return;

    names_and_events game = parseEventsFile(file_path);
    
    string destination = game.team_a_name + "_" + game.team_b_name;
    vector<Event> events = game.events;
    std::sort(events.begin(), events.end(), [](const Event & a, const Event & b) -> bool{ return a.get_time() < b.get_time(); });

    for (Event event : events){
        string body = "user: " + user.getUsername() + "\n" + event.toString();
        string receiptId = "report-" + std::to_string(user.getNextRID());
        SendFrame frame(destination, receiptId, body);
        string line = frame.toString();
        handler->sendLine(line);
    }
}

void InputManager::summary(string &game_name, string &reporter_user, string &file_path){
    User &user = handler->getUser();
    if (user.getSubscriptionId(game_name) == -1){
        std::cout << "User is not subscribed to this topic" << std::endl;
        return;
    }
    vector<Event> &events = user.getEvents(game_name, reporter_user);
    string output = "";
    vector<string> names = Frame::split(game_name, '_');

    output += names[0] + " vs " + names[1] + '\n' + "Game stats:\n";

    map<string, string> general_stats;
    map<string, string> team_a_stats;
    map<string, string> team_b_stats;
    string game_events = "Game event reports:\n";

    for (Event &event : events){
        game_events +=  event.summarize();
        std::cout << "hey1" << std::endl;

        for (auto pair : event.get_game_updates())
            general_stats[pair.first] = pair.second;
                std::cout << "hey2" << std::endl;

        for (auto pair : event.get_team_a_updates())
            team_a_stats[pair.first] = pair.second;
                std::cout << "hey3" << std::endl;

        for (auto pair : event.get_team_b_updates())
            team_b_stats[pair.first] = pair.second;  
                std::cout << "hey4" << std::endl;
  
    }
    std::cout << "hey5" << std::endl;

    output += "General stats:\n";
    for (auto pair : general_stats)
        output += pair.first + ": " + pair.second + '\n';
    output += names[0] + " stats:\n";
    for (auto pair : team_a_stats)
        output += pair.first + ": " + pair.second + '\n';
    output += names[1] + " stats:\n";
    for (auto pair : team_b_stats)
        output += pair.first + ": " + pair.second + '\n';
    std::cout << "hey6" << std::endl;

    output += game_events;
    std::cout << "hey7" << std::endl;

    std::ofstream outputFile(file_path, std::ofstream::trunc);
        std::cout << "hey8" << std::endl;

    outputFile << output << std::endl;
        std::cout << "hey9" << std::endl;

    outputFile.close();
        std::cout << "hey10" << std::endl;

    
}

void InputManager::logout(){
    User &user = handler->getUser();
    if (!user.getConnected()) return;

    string receiptId = "disconnect-" + std::to_string(user.getNextRID());
    DisconnectFrame frame(receiptId);
    string line = frame.toString();
    handler->sendLine(line);
}

