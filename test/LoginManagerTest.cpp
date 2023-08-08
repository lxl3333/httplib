#include <gtest/gtest.h>
#include "../src/include/httplib.h"
#include "../src/Server/LoginManager.h"

class LoginManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 在测试之前设置用户凭证
        login_manager_.user_credentials_ = {
            {"user1", "pass1"},
            {"user2", "pass2"},
            {"user3", "pass3"}
        };
    }

    void TearDown() override {
        // 在测试之后清理活动会话
        login_manager_.active_sessions_.clear();
    }

    LoginManager login_manager_;
};

TEST_F(LoginManagerTest, HandleLoginRequest_ValidCredentials) {
    httplib::Request req;
    httplib::Response res;

    req.params = {
        {"username", "user1"},
        {"password", "pass1"}
    };

    std::string token = login_manager_.handle_login_request(req, res);

    ASSERT_EQ(res.status, 200);
    ASSERT_EQ(res.get_header_value("Content-Type"), "text/plain");
    ASSERT_FALSE(token.empty());

    // 验证Token是否存在并且未过期
    ASSERT_TRUE(login_manager_.is_token_valid(token));
}

TEST_F(LoginManagerTest, HandleLoginRequest_InvalidCredentials) {
    httplib::Request req;
    httplib::Response res;

    req.params = {
        {"username", "user1"},
        {"password", "wrongpass"}
    };

    std::string token = login_manager_.handle_login_request(req, res);

    ASSERT_EQ(res.status, 401);
    ASSERT_EQ(res.get_header_value("Content-Type"), "text/plain");
    ASSERT_TRUE(token.empty());
}

TEST_F(LoginManagerTest, HandleLogoutRequest_ValidToken) {
    httplib::Request login_req;
    httplib::Response login_res;

    login_req.params = {
        {"username", "user1"},
        {"password", "pass1"}
    };

    std::string token = login_manager_.handle_login_request(login_req, login_res);

    httplib::Request logout_req;
    httplib::Response logout_res;

    logout_req.headers = {
        {"Authorization", token}
    };

    login_manager_.handle_logout_request(logout_req, logout_res);

    ASSERT_EQ(logout_res.status, 200);
    ASSERT_EQ(logout_res.get_header_value("Content-Type"), "text/plain");

    // 验证Token已被注销
    ASSERT_FALSE(login_manager_.is_token_valid(token));
}

TEST_F(LoginManagerTest, HandleLogoutRequest_InvalidToken) {
    httplib::Request req;
    httplib::Response res;

    req.headers = {
        {"Authorization", "invalid_token"}
    };

    login_manager_.handle_logout_request(req, res);

    ASSERT_EQ(res.status, 401);
    ASSERT_EQ(res.get_header_value("Content-Type"), "text/plain");
}

TEST_F(LoginManagerTest, TokenVerificationMiddleware_ValidToken) {
    httplib::Request login_req;
    httplib::Response login_res;

    login_req.params = {
        {"username", "user1"},
        {"password", "pass1"}
    };

    std::string token = login_manager_.handle_login_request(login_req, login_res);

    httplib::Request req;
    httplib::Response res;

    req.headers = {
        {"Authorization", token}
    };

    bool result = login_manager_.token_verification_middleware(req, res);

    ASSERT_TRUE(result);
    ASSERT_EQ(res.status, -1);  // 响应状态应该不会被修改
}

TEST_F(LoginManagerTest, TokenVerificationMiddleware_InvalidToken) {
    httplib::Request req;
    httplib::Response res;

    req.headers = {
        {"Authorization", "invalid_token"}
    };

    bool result = login_manager_.token_verification_middleware(req, res);

    ASSERT_FALSE(result);
    ASSERT_EQ(res.status, 401);
    ASSERT_EQ(res.get_header_value("Content-Type"), "text/plain");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
