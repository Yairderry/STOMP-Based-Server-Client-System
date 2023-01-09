#include "../include/Forum.h"
#include <algorithm>


Forum::Forum() : forum(map<string, map<string, vector<Event>>>{}){}

vector<Event> &Forum::getEvents(string &game_name, string &user){
    vector<Event> not_exist = vector<Event>{};

    if (forum.find(game_name) == forum.end())
        return not_exist;
    if (forum[game_name].find(user) == forum[game_name].end())
        return not_exist;

    vector<Event> &output = forum[game_name][user];

    return output;
}

void Forum::addEvent(Event &reported_event, string &game_name, string &reporter_user){
    if (forum.find(game_name) == forum.end())
        forum[game_name] = map<string, vector<Event>>{};
    if (forum[game_name].find(reporter_user) == forum[game_name].end())
        forum[game_name][reporter_user] = vector<Event>{};

    vector<Event> &user_reports = forum[game_name][reporter_user];
    user_reports.push_back(reported_event);
    std::sort(user_reports.begin(), user_reports.end(), [](const Event & a, const Event & b) -> bool{ return a.get_time() < b.get_time(); });
}