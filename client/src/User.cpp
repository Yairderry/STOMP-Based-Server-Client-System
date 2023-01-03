#include "../include/User.h"

User::User(string &username, string &password) : username(username), password(password), nextSubscriptionId(0), nextReceiptId(0), subscriptions(map<int, string>{}){}
        
string User::getUsername() {return username;}
string User::getPassword() {return password;}
int User::getNextSID() {return nextSubscriptionId;}
int User::getNextRID() {return nextReceiptId;}