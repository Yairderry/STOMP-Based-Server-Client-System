#include "../include/User.h"

User::User(string &username, string &password) : username(username), password(password), nextSubscriptionId(0), nextReceiptId(0), subscriptions(map<string, int>{}){}
        
string User::getUsername() {return username;}
string User::getPassword() {return password;}
int User::getNextSID() {return nextSubscriptionId;}
int User::getNextRID() {return nextReceiptId;}
int User::getSubscriptionId(string& id){
    return subscriptions.find(id) == subscriptions.end() ? -1 : subscriptions[id];
}
void User::addSubscription(string& id, int){
    subscriptions[id] = nextSubscriptionId++;
}

// void User::addGame(string& game_name, vector<Event> events){
//     games[game_name] = events;
// }