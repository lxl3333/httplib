#include "HandlerFactory.h"
#include "LoginManager.h"
#include "FileManageRequest.h"
#include "FileTransmission.h"
#include "../base/Singleton.h"
#include "../base/Logger.h"

std::function<void(const httplib::Request&, httplib::Response&)> HandlerFactory::createHandler(const Config::Handler& handler) {
    LOG_Debug(handler.url+" "+handler.type);
    if (handler.url == "/auth/login") {
        return [](const httplib::Request& req, httplib::Response& res) {
            Singleton<LoginManager>::getInstance().handle_login_request(req,res);
            return ;
        };
    } else if (handler.url == "/auth/logout") {
        return [](const httplib::Request& req, httplib::Response& res) {
            Singleton<LoginManager>::getInstance().handle_logout_request(req,res);
            return ;
        };
    } else if (handler.url == "/auth/verify_token") {
        return [](const httplib::Request& req, httplib::Response& res) {
            Singleton<LoginManager>::getInstance().token_verification_middleware(req,res);
        };
    }
    // } else if (handler.url == "/files/Dir") {
    //     return [](const httplib::Request& req, httplib::Response& res) {
    //         Singleton<LoginManager>::getInstance().token_verification_middleware(req,res);
    //         return true;  // or false, depending on the result
    //     };
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
    return [](const httplib::Request& req, httplib::Response& res) {
        res.status = 404;
    };
}
