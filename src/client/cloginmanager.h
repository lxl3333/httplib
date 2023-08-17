#ifndef CLOGINMANAGER_H
#define CLOGINMANAGER_H

#include <string>
#include "../include/httplib.h"

class CLoginManager {
public:
    CLoginManager(std::shared_ptr<httplib::Client> client);

    bool Login(const std::string username, const std::string password);
    bool Logout();
    bool IsLoggedIn() const;
    std::string GetCurrentUser() const;
    const std::string& GetToken() const;

private:
    std::shared_ptr<httplib::Client> client_;
    std::string username_;
    std::string password_;
    std::string token_;
    bool loggedIn_;
};

#endif  // CLOGINMANAGER_H
