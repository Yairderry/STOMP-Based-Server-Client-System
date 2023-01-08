#include "../include/Forum.h"
#include <algorithm>


Forum::Forum() : forum(map<string, map<string, vector<Event>>>{}){}

vector<Event> Forum::getEvents(string &game_name, string &user){
        return forum[game_name][user];
}

void Forum::addEvent(Event &reported_event, string &game_name, string &reporter_user){
    if (forum.find(game_name) == forum.end())
        forum[game_name] = map<string, vector<Event>>{};
    if (forum[game_name].find(reporter_user) == forum[game_name].end())
        forum[game_name][reporter_user] = vector<Event>{};

    forum[game_name][reporter_user].push_back(reported_event);
    std::sort(forum[game_name][reporter_user].begin(), forum[game_name][reporter_user].end(), [](const Event & a, const Event & b) -> bool{ return a.get_time() < b.get_time(); });
}