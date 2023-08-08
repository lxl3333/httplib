#include <gtest/gtest.h>
#include "../src/include/httplib.h"
#include "../src/Server/FileManageRequest.h"


TEST(FileManageRequestTest, HandleCreateDirectory_Success) {
    std::string rootPath = "/path/to/root/directory";
    FileManager fileManager(rootPath);
    FileManageRequest fileManageRequest(rootPath);
    httplib::Request req;
    httplib::Response res;

    req.body = "test_directory";
    bool createDirectoryResult = fileManageRequest.handleCreateDirectory(req, res);

    EXPECT_TRUE(createDirectoryResult);
    EXPECT_EQ(res.status, 200);
    EXPECT_EQ(res.get_header_value("Content-Type"), "text/plain");
    EXPECT_EQ(res.body, "Directory created successfully");
}

TEST(FileManageRequestTest, HandleCreateDirectory_Failure) {
    std::string rootPath = "/path/to/root/directory";
    FileManager fileManager(rootPath);
    FileManageRequest fileManageRequest(rootPath);
    httplib::Request req;
    httplib::Response res;

    req.body = "test_directory";
    // 模拟创建目录失败的情况
    bool createDirectoryResult = false;
    bool expectedResult = fileManageRequest.handleCreateDirectory(req, res);

    EXPECT_EQ(createDirectoryResult, expectedResult);
    EXPECT_EQ(res.status, 500);
    EXPECT_EQ(res.get_header_value("Content-Type"), "text/plain");
    EXPECT_EQ(res.body, "Failed to create directory");
}

// Add more test cases for other handlers

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
