#pragma once
#include <string>
#include <map>
#include <vector>
#include "Event.h"
#include "Forum.h"

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
        map<string, int> subsByName;
        map<int, string> subsById;
        bool connected = false;
        Forum forum;

    public:
        User(string&, string&);
        User *clone();

        string getUsername();
        string getPassword();
        int getNextSID();
        int getNextRID();
        int getSubscriptionId(string&);
        string getChannelById(int);
        bool addSubscription(string&, int);
        bool removeSubscription(string&);
        void toggleConnected();
        bool getConnected();
        void addEvent(Event &, string &, string &);
        void getEvents(vector<Event> &, string &, string &);
        
};