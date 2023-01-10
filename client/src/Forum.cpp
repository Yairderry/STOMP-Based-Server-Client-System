#include "../include/Forum.h"
#include <algorithm>
#include <boost/optional.hpp>


Forum::Forum() : forum(map<string, map<string, vector<Event>>>{}){}

void Forum::getEvents(vector<Event> &events, string &game_name, string &user){
    if (forum.find(game_name) == forum.end())
        return;
    if (forum[game_name].find(user) == forum[game_name].end())
        return;

    events = forum[game_name][user];
}

void Forum::addEvent(Event &reported_event, string &game_name, string &reporter_user){
    if (forum.find(game_name) == forum.end())
        forum[game_name] = map<string, vector<Event>>{};
    if (forum[game_name].find(reporter_user) == forum[game_name].end())
        forum[game_name][reporter_user] = vector<Event>{};

    vector<Event> &user_reports = forum[game_name][reporter_user];
    user_reports.push_back(reported_event);
}