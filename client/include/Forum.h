#pragma once
#include <map>
#include <vector>
#include <string>
#include "Event.h"

// class Frame;
// class ConnectionHandler;
using std::map;
using std::vector;
using std::string;

class Forum
{
private:
    map<string, map<string, vector<Event>>> forum;
public:
    Forum();
    void getEvents(vector<Event> &, string&, string&);
    void addEvent(Event &, string &, string &);

};
