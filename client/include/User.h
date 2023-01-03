#pragma once
#include <string>
#include <map>

using std::string;
using std::map;

class User
{
    private:
        string username;
        string password;
        int nextSubscriptionId;
        int nextReceiptId;
        map<int, string> subscriptions;

    public:
        User(string&, string&);

        string getUsername();
        string getPassword();
        int getNextSID();
        int getNextRID();
        
};