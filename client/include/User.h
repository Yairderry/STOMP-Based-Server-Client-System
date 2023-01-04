#pragma once
#include <string>
#include <map>
#include <vector>
#include "Event.h"

using std::string;
using std::map;
using std::vector;

class User
{
    private:
        string username;
        string password;
        int nextSubscriptionId;
        int nextReceiptId;
        map<string, int> subscriptions;
        map<string, vector<Event>> games;
        bool connected = false;

    public:
        User(string&, string&);

        string getUsername();
        string getPassword();
        int getNextSID();
        int getNextRID();
        int getSubscriptionId(string&);
        void addSubscription(string&, int);
        void toggleConnected();
        bool getConnected();
        
};