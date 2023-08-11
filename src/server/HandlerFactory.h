#ifndef HANDLER_FACTORY_H
#define HANDLER_FACTORY_H

#include <functional>
#include <memory>
#include "../include/httplib.h"
#include "../base/Config.h"


class LoginManager;
class FileTransmission;
class FileManageRequest;

class HandlerFactory {
public:
    static std::function<void(const httplib::Request&, httplib::Response&)> createHandler(const Config::Handler& handler);
};

#endif  // HANDLER_FACTORY_H
