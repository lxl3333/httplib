#include "../src/Server/HandlerFactory.h"
#include "../src/Server/LoginManager.h"
#include "../src/Server/FileTransmission.h"
#include "../src/Server/FileManageRequest.h"
#include "../src/include/httplib.h"
#include <gtest/gtest.h>


class MockLoginManager : public LoginManager {
public:
    std::string handle_login_request(const httplib::Request& req, httplib::Response& res) override {
        return "Login successful";
    }

    void handle_logout_request(const httplib::Request& req, httplib::Response& res) override {
        // Do nothing for the mock implementation
    }

    bool token_verification_middleware(const httplib::Request& req, httplib::Response& res) override {
        return true;
    }
};

class MockFileTransmission : public FileTransmission {
public:
    bool SendFixedFile(const httplib::Request& req, httplib::Response& res) override {
        return true;
    }

    bool SendChunkedFile(const httplib::Request& req, httplib::Response& res) override {
        return true;
    }

    bool Sendc(const httplib::Request& req, httplib::Response& res) override {
        return true;
    }

    bool ReceiveFixedFile(const httplib::Request& req, httplib::Response& res) override {
        return true;
    }

    bool ReceiveChunkedFile(const httplib::Request& req, httplib::Response& res) override {
        return true;
    }

    bool ReceiveDirectory(const httplib::Request& req, httplib::Response& res) override {
        return true;
    }
};

class MockFileManageRequest : public FileManageRequest {
public:
    bool handleCreateDirectory(const httplib::Request& req, httplib::Response& res) override {
        return true;
    }

    bool handleRenameFile(const httplib::Request& req, httplib::Response& res) override {
        return true;
    }

    bool handleMoveFile(const httplib::Request& req, httplib::Response& res) override {
        return true;
    }

    bool handleCopyFile(const httplib::Request& req, httplib::Response& res) override {
        return true;
    }
};

TEST(HandlerFactoryTest, CreateLoginHandler) {
    MockLoginManager loginManager;
    httplib::Request req;
    httplib::Response res;

    auto handler = HandlerFactory::createLoginHandler(&loginManager);
    handler(req, res);

    EXPECT_EQ(res.body, "Login successful");
    EXPECT_EQ(res.get_header_value("Content-Type"), "text/plain");
}

TEST(HandlerFactoryTest, CreateLogoutHandler) {
    MockLoginManager loginManager;
    httplib::Request req;
    httplib::Response res;

    auto handler = HandlerFactory::createLogoutHandler(&loginManager);
    handler(req, res);
}

TEST(HandlerFactoryTest, CreateFileTransmissionHandler) {
    MockFileTransmission fileTransmission;
    httplib::Request req;
    httplib::Response res;

    auto handler = HandlerFactory::createFileTransmissionHandler(&fileTransmission);
    // Call the handler method you want to test
    handler.SendFixedFile(req, res);
    // Add expectations and assertions based on the behavior of the handler method
    EXPECT_TRUE(res.status == 200);
    EXPECT_EQ(res.get_header_value("Content-Type"), "text/plain");
}

TEST(HandlerFactoryTest, CreateFileManageRequestHandler) {
    MockFileManageRequest fileManageRequest;
    httplib::Request req;
    httplib::Response res;

    auto handler = HandlerFactory::createFileManageRequestHandler(&fileManageRequest);
    // Call the handler method you want to test
    handler.handleCreateDirectory(req, res);
    // Add expectations and assertions based on the behavior of the handler method
    EXPECT_TRUE(res.status == 200);
    EXPECT_EQ(res.get_header_value("Content-Type"), "text/plain");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
