#ifndef LOGIN_MANAGER_H
#define LOGIN_MANAGER_H

#include <string>
#include <unordered_map>
#include <mutex>
#include <openssl/rand.h>

#include "../include/httplib.h"

struct Session {
    std::string user_id;
    std::string token;
    std::chrono::system_clock::time_point expires_at;
};

class LoginManager {
public:
    LoginManager();

    bool handle_login_request(const httplib::Request& req, httplib::Response& res);
    void handle_logout_request(const httplib::Request& req, httplib::Response& res);
    bool token_verification_middleware(const httplib::Request& req, httplib::Response& res);
    void set_user_credentials(const std::string& username ,const std::string& password);
    void set_user_credentials(const std::unordered_map<std::string,std::string>& credentials);
private:
    std::unordered_map<std::string, std::string> user_credentials_;
    std::unordered_map<std::string, Session> active_sessions_;
    std::mutex mutex_;

    std::string generate_token();
    bool is_token_valid(const std::string& token);
};

#endif  // LOGIN_MANAGER_H
