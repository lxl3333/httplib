#include "HandlerFactory.h"
#include "LoginManager.h"
#include "FileTransmission.h"
#include "FileManageRequest.h"

std::function<void(const httplib::Request&, httplib::Response&)> HandlerFactory::createLoginHandler(LoginManager* loginManager) {
    return [=](const httplib::Request& req, httplib::Response& res) {
        std::string result = loginManager->handle_login_request(req, res);
        res.set_content(result, "text/plain");
    };
}

std::function<void(const httplib::Request&, httplib::Response&)> HandlerFactory::createLogoutHandler(LoginManager* loginManager) {
    return [=](const httplib::Request& req, httplib::Response& res) {
        loginManager->handle_logout_request(req, res);
    };
}

std::function<bool(const httplib::Request&, httplib::Response&)> HandlerFactory::createTokenVerificationMiddleware(LoginManager* loginManager) {
    return [=](const httplib::Request& req, httplib::Response& res) {
        return loginManager->token_verification_middleware(req, res);
    };
}

std::function<bool(const httplib::Request&, httplib::Response&)> HandlerFactory::createSendFixedFileHandler(FileTransmission* fileTransmission) {
    return [=](const httplib::Request& req, httplib::Response& res) {
        return fileTransmission->SendFixedFile(req, res);
    };
}

std::function<bool(const httplib::Request&, httplib::Response&)> HandlerFactory::createSendChunkedFileHandler(FileTransmission* fileTransmission) {
    return [=](const httplib::Request& req, httplib::Response& res) {
        return fileTransmission->SendChunkedFile(req, res);
    };
}

std::function<bool(const httplib::Request&, httplib::Response&)> HandlerFactory::createSendcHandler(FileTransmission* fileTransmission) {
    return [=](const httplib::Request& req, httplib::Response& res) {
        return fileTransmission->Sendc(req, res);
    };
}

std::function<bool(const httplib::Request&, httplib::Response&)> HandlerFactory::createReceiveFixedFileHandler(FileTransmission* fileTransmission) {
    return [=](const httplib::Request& req, httplib::Response& res) {
        return fileTransmission->ReceiveFixedFile(req, res);
    };
}

std::function<bool(const httplib::Request&, httplib::Response&)> HandlerFactory::createReceiveChunkedFileHandler(FileTransmission* fileTransmission) {
    return [=](const httplib::Request& req, httplib::Response& res) {
        return fileTransmission->ReceiveChunkedFile(req, res);
    };
}

std::function<bool(const httplib::Request&, httplib::Response&)> HandlerFactory::createReceiveDirectoryHandler(FileTransmission* fileTransmission) {
    return [=](const httplib::Request& req, httplib::Response& res) {
        return fileTransmission->ReceiveDirectory(req, res);
    };
}

std::function<bool(const httplib::Request&, httplib::Response&)> HandlerFactory::createCreateDirectoryHandler(FileManageRequest* fileManageRequest) {
    return [=](const httplib::Request& req, httplib::Response& res) {
        return fileManageRequest->handleCreateDirectory(req, res);
    };
}

std::function<bool(const httplib::Request&, httplib::Response&)> HandlerFactory::createRenameFileHandler(FileManageRequest* fileManageRequest) {
    return [=](const httplib::Request& req, httplib::Response& res) {
        return fileManageRequest->handleRenameFile(req, res);
    };
}

std::function<bool(const httplib::Request&, httplib::Response&)> HandlerFactory::createMoveFileHandler(FileManageRequest* fileManageRequest) {
    return [=](const httplib::Request& req, httplib::Response& res) {
        return fileManageRequest->handleMoveFile(req, res);
    };
}

std::function<bool(const httplib::Request&, httplib::Response&)> HandlerFactory::createCopyFileHandler(FileManageRequest* fileManageRequest) {
    return [=](const httplib::Request& req, httplib::Response& res) {
        return fileManageRequest->handleCopyFile(req, res);
    };
}
