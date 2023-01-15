#include "../include/Event.h"
#include "../include/Frame.h"
#include "../include/json.hpp"
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <sstream>

using json = nlohmann::json;

Event::Event(std::string team_a_name, std::string team_b_name, std::string name, int time,
             std::map<std::string, std::string> game_updates, std::map<std::string, std::string> team_a_updates,
             std::map<std::string, std::string> team_b_updates, std::string discription)
        : team_a_name(std::move(team_a_name)), team_b_name(std::move(team_b_name)), name(std::move(name)),
          time(time), game_updates(std::move(game_updates)), team_a_updates(std::move(team_a_updates)),
          team_b_updates(std::move(team_b_updates)), description(std::move(discription)) {
}

Event::~Event()
= default;

const std::string &Event::get_team_a_name() const {
    return this->team_a_name;
}

const std::string &Event::get_team_b_name() const {
    return this->team_b_name;
}

const std::string &Event::get_name() const {
    return this->name;
}

int Event::get_time() const {
    return this->time;
}

const std::map<std::string, std::string> &Event::get_game_updates() const {
    return this->game_updates;
}

const std::map<std::string, std::string> &Event::get_team_a_updates() const {
    return this->team_a_updates;
}

const std::map<std::string, std::string> &Event::get_team_b_updates() const {
    return this->team_b_updates;
}

const std::string &Event::get_discription() const {
    return this->description;
}

Event::Event(const std::string &frame_body) : team_a_name(""), team_b_name(""), name(""), time(0),
                                              game_updates(map<string, string>{}),
                                              team_a_updates(map<string, string>{}),
                                              team_b_updates(map<string, string>{}), description("") {
    vector<string> report_lines = Frame::split(frame_body, '\n');
    team_a_name = Frame::split(report_lines[1], ':')[1].substr(1);
    team_b_name = Frame::split(report_lines[2], ':')[1].substr(1);
    name = Frame::split(report_lines[3], ':')[1].substr(1);
    time = std::stoi(Frame::split(report_lines[4], ':')[1].substr(1));

    unsigned int i = 6;

    while (report_lines[i] != "team a updates:") {
        vector<string> pair = Frame::split(report_lines[i], ':');
        boost::trim(pair[0]);
        boost::trim(pair[1]);
        game_updates[pair[0]] = pair[1];
        i++;
    }
    while (report_lines[++i] != "team b updates:") {
        vector<string> pair = Frame::split(report_lines[i], ':');
        boost::trim(pair[0]);
        boost::trim(pair[1]);
        team_a_updates[pair[0]] = pair[1];
    }

    while (report_lines[++i] != "description:") {
        vector<string> pair = Frame::split(report_lines[i], ':');
        boost::trim(pair[0]);
        boost::trim(pair[1]);
        team_b_updates[pair[0]] = pair[1];
    }

    while (++i < report_lines.size()) {
        description += report_lines[i];
    }
}

std::string Event::toString() {
    std::string output;

    output += "team a: " + this->team_a_name + "\n";
    output += "team b: " + this->team_b_name + "\n";
    output += "event name: " + this->name + "\n";
    output += "time: " + std::to_string(this->time) + "\n";

    output += "general game updates:\n";
    for (auto &update: this->game_updates) {
        output += "\t" + update.first + ": " + update.second + "\n";
    }

    output += "team a updates:\n";
    for (auto &update: this->team_a_updates)
        output += "\t" + update.first + ": " + update.second + "\n";

    output += "team b updates:\n";
    for (auto &update: this->team_b_updates)
        output += "\t" + update.first + ": " + update.second + "\n";

    output += "description:\n" + this->description;

    return output;
}

string Event::summarize() {
    std::string output;
    output += std::to_string(this->time) + " - " + this->name + ":\n\n" + this->description + "\n\n\n";
    return output;
}

names_and_events parseEventsFile(std::string json_path) {
    std::ifstream f(json_path);
    json data = json::parse(f);

    std::string team_a_name = data["team a"];
    std::string team_b_name = data["team b"];

    // run over all the events and convert them to Event objects
    std::vector<Event> events;
    for (auto &event: data["events"]) {
        std::string name = event["event name"];
        int time = event["time"];
        std::string description = event["description"];
        std::map<std::string, std::string> game_updates;
        std::map<std::string, std::string> team_a_updates;
        std::map<std::string, std::string> team_b_updates;
        for (auto &update: event["general game updates"].items()) {
            if (update.value().is_string())
                game_updates[update.key()] = update.value();
            else
                game_updates[update.key()] = update.value().dump();
        }

        for (auto &update: event["team a updates"].items()) {
            if (update.value().is_string())
                team_a_updates[update.key()] = update.value();
            else
                team_a_updates[update.key()] = update.value().dump();
        }

        for (auto &update: event["team b updates"].items()) {
            if (update.value().is_string())
                team_b_updates[update.key()] = update.value();
            else
                team_b_updates[update.key()] = update.value().dump();
        }

        events.push_back(
                Event(team_a_name, team_b_name, name, time, game_updates, team_a_updates, team_b_updates, description));
    }
    names_and_events events_and_names{team_a_name, team_b_name, events};

    return events_and_names;
}