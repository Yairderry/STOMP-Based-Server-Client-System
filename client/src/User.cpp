#include "../include/User.h"

User::User(string &username, string &password) : username(username), password(password), nextSubscriptionId(0), nextReceiptId(0), subsByName(map<string, int>{}), subsById(map<int, string>{}), forum(Forum()){}
        
string User::getUsername() {return username;}
string User::getPassword() {return password;}
int User::getNextSID() {return nextSubscriptionId;}
int User::getNextRID() {return nextReceiptId++;}
int User::getSubscriptionId(string& game_name){
    return subsByName.find(game_name) == subsByName.end() ? -1 : subsByName[game_name];
}
string User::getChannelById(int id){
    return subsById.find(id) == subsById.end() ? "" : subsById[id];
}
void User::addSubscription(string& game_name, int){
    subsById[nextSubscriptionId] = game_name;
    subsByName[game_name] = nextSubscriptionId++;
}
void User::removeSubscription(string& game_name){
    int id = subsByName[game_name];
    subsByName.erase(game_name);
    subsById.erase(id);
}

void User::toggleConnected(){
    connected = !connected;
}

bool User::getConnected(){
    return connected;
}

void User::addEvent(Event &reported_event, string &game_name, string &reporter_user){
    forum.addEvent(reported_event, game_name, reporter_user);
}