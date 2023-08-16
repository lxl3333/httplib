#include "Config.h"
#include <fstream>
#include <jsoncpp/json/json.h>

Config::Config(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open config file");
    }
    Json::Value root;
    file >> root;
    address_ = root["address"].asString();
    run_user_ = root["runUser"].asString();
    concurrency_limit_ = root["concurrencyLimit"].asInt();
    keepalive_limit_ = root["keepaliveLimit"].asInt();
    auto handlers = root["handler"];
    for (auto& handler : handlers) {
        std::string url = handler.get("url", "").asString();
        std::string type = handler.get("handler", "").asString();
        std::string dir = handler.get("dir", "").asString();
        handlers_.push_back({url, type, dir});
    }
    auto credentials=root["credential"];
    for(auto & credential:credentials){
        std::string username=credential.get("username","").asString();
        std::string password=credential.get("password","").asString();
        credentials_.push_back({username,password});
    }
}

const std::string& Config::address() const {
    return address_;
}

const std::string& Config::run_user() const {
    return run_user_;
}

int Config::concurrency_limit() const {
    return concurrency_limit_;
}

int Config::keepalive_limit() const {
    return keepalive_limit_;
}

const std::vector<Config::Handler>& Config::handlers() const {
    return handlers_;
}

const std::vector<Config::Credential>& Config::credentials() const {
    return credentials_;
}
