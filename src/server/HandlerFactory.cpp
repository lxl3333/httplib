#include "HandlerFactory.h"
#include "LoginManager.h"
#include "FileTransmission.h"
#include "../base/Singleton.h"
#include "../base/Logger.h"

HandlerInfo HandlerFactory::createHandler(const Config::Handler &handler)
{
    LOG_Debug(handler.url + " " + handler.type + " " + handler.method);

    HandlerInfo handler_info;

    if (handler.url == "/auth/login")
    {
        handler_info.handler_function = [](const httplib::Request &req, httplib::Response &res)
        {
            Singleton<LoginManager>::getInstance().handle_login_request(req, res);
            return;
        };
    }
    else if (handler.url == "/auth/logout")
    {
        handler_info.handler_function = [](const httplib::Request &req, httplib::Response &res)
        {
            Singleton<LoginManager>::getInstance().handle_logout_request(req, res);
            return;
        };
    }
    else if (handler.url == "/auth/verify_token")
    {
        handler_info.handler_function = [](const httplib::Request &req, httplib::Response &res)
        {
            Singleton<LoginManager>::getInstance().token_verification_middleware(req, res);
        };
    }
    else if (handler.url == "/files/Dirlists")
    {
        handler_info.handler_function = [handler](const httplib::Request &req, httplib::Response &res)
        {
            FileManageRequest(handler.dir).handleListFiles(req, res);
            return true; // or false, depending on the result
        };
    }
    else
    {
        handler_info.handler_function = [](const httplib::Request &req, httplib::Response &res)
        {
            res.status = 404;
        };
    }
    // } else if (handler.url == "/files/upload/FixedFile") {
    //     return [](const httplib::Request& req, httplib::Response& res) {
    //         Singleton<LoginManager>::getInstance().token_verification_middleware(req,res);
    //         return true;  // or false, depending on the result
    //     };
    // } else if (handler.url == "/files/upload/ChunkedFile") {
    //     return [](const httplib::Request& req, httplib::Response& res) {
    //         Singleton<LoginManager>::getInstance().token_verification_middleware(req,res);
    //         return true;  // or false, depending on the result
    //     };
    // } else if (handler.url == "/files/download/FixedFile") {
    //     return [](const httplib::Request& req, httplib::Response& res) {
    //         Singleton<LoginManager>::getInstance().token_verification_middleware(req,res);
    //         return true;  // or false, depending on the result
    //     };
    // } else if (handler.url == "/files/download/ChunkedFile") {
    //     return [](const httplib::Request& req, httplib::Response& res) {
    //         Singleton<LoginManager>::getInstance().token_verification_middleware(req,res);
    //         return true;  // or false, depending on the result
    //     };
    // } else if (handler.url == "/files/delete") {
    //     return [](const httplib::Request& req, httplib::Response& res) {
    //         Singleton<LoginManager>::getInstance().token_verification_middleware(req,res);
    //         return true;  // or false, depending on the result
    //     };
    // } else if (handler.url == "/files/create-directory") {
    //     return [](const httplib::Request& req, httplib::Response& res) {
    //         Singleton<LoginManager>::getInstance().token_verification_middleware(req,res);
    //         return true;  // or false, depending on the result
    //     };
    // } else if (handler.url == "/files/rename") {
    //     return [](const httplib::Request& req, httplib::Response& res) {
    //         Singleton<LoginManager>::getInstance().token_verification_middleware(req,res);
    //         return true;  // or false, depending on the result
    //     };
    // } else if (handler.url == "/files/move") {
    //     return [](const httplib::Request& req, httplib::Response& res) {
    //         Singleton<LoginManager>::getInstance().token_verification_middleware(req,res);
    //         return true;  // or false, depending on the result
    //     };
    // } else if (handler.url == "/files/copy") {
    //     return [](const httplib::Request& req, httplib::Response& res) {
    //         Singleton<LoginManager>::getInstance().token_verification_middleware(req,res);
    //         return true;  // or false, depending on the result
    //     };
    // }

    // Default handler
    if (handler.method == "GET")
    {
        handler_info.http_method = HttpMethod::GET;
    }
    else if (handler.method == "POST")
    {
        handler_info.http_method = HttpMethod::POST;
    }
    else if (handler.method == "PUT")
    {
        handler_info.http_method = HttpMethod::PUT;
    }
    else if (handler.method == "DELETE")
    {
        handler_info.http_method = HttpMethod::DELETE;
    }
    return handler_info;
}
