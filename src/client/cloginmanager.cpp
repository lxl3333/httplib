#include "cloginmanager.h"
#include "../base/Logger.h"
#include <iostream>

CLoginManager::CLoginManager(std::shared_ptr<httplib::Client> client) :client_(client), loggedIn_(false) {}

bool CLoginManager::Login(const std::string username, const std::string password) {

    httplib::Params params;
    params.emplace("username", username);
    params.emplace("password", password);

    auto res = client_->Post("/auth/login", params);
    if(!res) {
        LOG_Debug("请求无响应");
        return false;
    }
    if (res->status == 200) {
        LOG_Debug("请求响应成功");
        loggedIn_ = true;
        username_ = username;
        password_ = password;
        token_ = res->body;
        return true;
    }else
    {
        LOG_Debug("请求响应失败");
    }

    return false;
}

bool CLoginManager::Logout() {
    LOG_Info("ClognManager::logout");

    if (!loggedIn_) {
        return true;  // Already logged out
    }
    httplib::Headers headers = {{"Authorization", token_}};

    auto res = client_->Post("/auth/logout", headers);

    if (res && res->status == 200) {
        loggedIn_ = false;
        username_.clear();
        token_.clear();
        return true;
    }

    return false;
}

bool CLoginManager::IsLoggedIn() const {
    return loggedIn_;
}

std::string CLoginManager::GetCurrentUser() const {
    return username_;
}

std::string CLoginManager::GetToken() const {
    return token_;
}
