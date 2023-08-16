#include "LoginManager.h"
#include "../base/Logger.h"
LoginManager::LoginManager() {}

bool LoginManager::handle_login_request(const httplib::Request& req, httplib::Response& res) {
    std::string username = req.get_param_value("username");
    std::string password = req.get_param_value("password");
    LOG_Info("/auth/login: "+username+":"+password);

    std::lock_guard<std::mutex> lock(mutex_);

    if (user_credentials_.count(username) > 0 && user_credentials_[username] == password) {
        std::string token = generate_token();

        Session session;
        session.user_id = username;
        session.token = token;
        session.expires_at = std::chrono::system_clock::now() + std::chrono::hours(1);  // Token有效期为1小时

        active_sessions_[token] = session;

        res.status = 200 ;
        res.set_content(token, "text/plain");
        return true;
    }

    res.status = 401;
    res.set_content("Invalid credentials", "text/plain");
    return false;
}

void LoginManager::handle_logout_request(const httplib::Request& req, httplib::Response& res) {
    std::string token = req.get_header_value("Authorization");
    LOG_Debug("/auth/login Authorization:"+token);
    std::lock_guard<std::mutex> lock(mutex_);

    if (active_sessions_.count(token) > 0) {
        active_sessions_.erase(token);
        res.status = 200;
        res.set_content("Logout successful", "text/plain");
    } else {
        res.status = 401;
        res.set_content("Invalid Token", "text/plain");
    }
}

bool LoginManager::token_verification_middleware(const httplib::Request& req, httplib::Response& res) {
    std::string token = req.get_header_value("Authorization");

    std::lock_guard<std::mutex> lock(mutex_);

    if (!is_token_valid(token)) {
        res.status = 401;
        res.set_content("Invalid Token", "text/plain");
        return false;
    }

    return true;
}

std::string LoginManager::generate_token() {
    constexpr size_t kTokenLength = 32;  // Token长度为32个字符

    std::string token;
    token.resize(kTokenLength);

    if (RAND_bytes(reinterpret_cast<unsigned char*>(&token[0]), kTokenLength) != 1) {
        // Token生成失败，可以根据需要进行错误处理
        // 在此示例中，简单地返回空的Token
        return "";
    }

    return token;
}

bool LoginManager::is_token_valid(const std::string& token) {
    auto it = active_sessions_.find(token);
    if (it == active_sessions_.end()) {
        return false;  // Token不存在
    }

    const Session& session = it->second;
    if (session.expires_at <= std::chrono::system_clock::now()) {
        active_sessions_.erase(it);  // Token已过期，从活动会话中移除
        return false;
    }

    return true;
}

void LoginManager::set_user_credentials(const std::string& username ,const std::string& password){
    LOG_Info("inser user:"+username);
    std::lock_guard<std::mutex> lock(mutex_);
    user_credentials_[username]=password;
}

void LoginManager::set_user_credentials(const std::unordered_map<std::string,std::string>& credentials){
    std::lock_guard<std::mutex> lock(mutex_);
    user_credentials_.insert(credentials.begin(),credentials.end());
}
