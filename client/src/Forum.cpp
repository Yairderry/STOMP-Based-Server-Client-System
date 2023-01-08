#include "../include/Forum.h"
#include <algorithm>


Forum::Forum() : forum(map<string, map<string, vector<Event>>>{}){}

vector<Event> Forum::getEvents(string &game_name, string &user){
        return forum[game_name][user];
}

void Forum::addEvent(Event &reported_event, string &game_name, string &reporter_user){
    vector<Event> events = forum[game_name][reporter_user];
    events.push_back(reported_event);
    std::sort(events.begin(), events.end(), [](const Event & a, const Event & b) -> bool{ return a.get_time() < b.get_time(); });
}