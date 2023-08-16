#ifndef HANDLER_FACTORY_H
#define HANDLER_FACTORY_H

#include <functional>
#include <memory>
#include "../include/httplib.h"
#include "../base/Config.h"

class LoginManager;
class FileTransmission;
class FileManageRequest;

enum class HttpMethod
{
    GET,
    POST,
    PUT,
    DELETE
    // 可以继续添加其他HTTP请求方法
};

struct HandlerInfo
{
    std::function<void(const httplib::Request &, httplib::Response &)> handler_function;
    std::string http_method;
};

class HandlerFactory
{
public:
    static HandlerInfo createHandler(const Config::Handler &handler);
};

#endif // HANDLER_FACTORY_H
