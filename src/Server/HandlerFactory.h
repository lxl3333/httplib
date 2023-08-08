#ifndef HANDLER_FACTORY_H
#define HANDLER_FACTORY_H

#include <functional>
#include <memory>
#include "../include/httplib.h"

class LoginManager;
class FileTransmission;
class FileManageRequest;

class HandlerFactory {
public:
    static std::function<void(const httplib::Request&, httplib::Response&)> createLoginHandler(LoginManager* loginManager);
    static std::function<void(const httplib::Request&, httplib::Response&)> createLogoutHandler(LoginManager* loginManager);
    static std::function<bool(const httplib::Request&, httplib::Response&)> createTokenVerificationMiddleware(LoginManager* loginManager);
    static std::function<bool(const httplib::Request&, httplib::Response&)> createSendFixedFileHandler(FileTransmission* fileTransmission);
    static std::function<bool(const httplib::Request&, httplib::Response&)> createSendChunkedFileHandler(FileTransmission* fileTransmission);
    static std::function<bool(const httplib::Request&, httplib::Response&)> createSendcHandler(FileTransmission* fileTransmission);
    static std::function<bool(const httplib::Request&, httplib::Response&)> createReceiveFixedFileHandler(FileTransmission* fileTransmission);
    static std::function<bool(const httplib::Request&, httplib::Response&)> createReceiveChunkedFileHandler(FileTransmission* fileTransmission);
    static std::function<bool(const httplib::Request&, httplib::Response&)> createReceiveDirectoryHandler(FileTransmission* fileTransmission);
    static std::function<bool(const httplib::Request&, httplib::Response&)> createCreateDirectoryHandler(FileManageRequest* fileManageRequest);
    static std::function<bool(const httplib::Request&, httplib::Response&)> createRenameFileHandler(FileManageRequest* fileManageRequest);
    static std::function<bool(const httplib::Request&, httplib::Response&)> createMoveFileHandler(FileManageRequest* fileManageRequest);
    static std::function<bool(const httplib::Request&, httplib::Response&)> createCopyFileHandler(FileManageRequest* fileManageRequest);
};

#endif  // HANDLER_FACTORY_H
