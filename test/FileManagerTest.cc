#include "../src/base/FileManager.h"
#include "gtest/gtest.h"

// Test fixture for FileManager class
class FileManagerTest : public ::testing::Test {
protected:
    FileManagerTest() : fileManager_("test_dir/") {}

    void SetUp() override {
        // Create a test directory
        fs::create_directory("test_dir/");
    }

    void TearDown() override {
        // Remove the test directory and all its contents
        fs::remove_all("test_dir/");
    }

    FileManager fileManager_;
};

TEST_F(FileManagerTest, FileExists) {
    // Create a test file
    std::ofstream outfile("test_dir/test.txt");
    outfile << "test content" << std::endl;
    outfile.close();

    EXPECT_TRUE(fileManager_.FileExists("test.txt"));
    EXPECT_FALSE(fileManager_.FileExists("nonexistent.txt"));
}

TEST_F(FileManagerTest, CreateDirectory) {
    EXPECT_TRUE(fileManager_.CreateDirectory("subdir/"));
    EXPECT_TRUE(fs::exists("test_dir/subdir/"));
}

TEST_F(FileManagerTest, CreateFile) {
    EXPECT_TRUE(fileManager_.CreateFile("test.txt"));
    EXPECT_TRUE(fs::exists("test_dir/test.txt"));
}

TEST_F(FileManagerTest, CopyFile) {
    // Create a test file
    std::ofstream outfile("test_dir/test.txt");
    outfile << "test content" << std::endl;
    outfile.close();

    EXPECT_TRUE(fileManager_.CopyFile("test.txt", "test_copy.txt"));
    EXPECT_TRUE(fs::exists("test_dir/test_copy.txt"));
}

TEST_F(FileManagerTest, CopyDirectory) {
    // Create a test directory and file
    fs::create_directory("test_dir/subdir/");
    std::ofstream outfile("test_dir/subdir/test.txt");
    outfile << "test content" << std::endl;
    outfile.close();

    EXPECT_TRUE(fileManager_.CopyDirectory("subdir/", "subdir_copy/"));
    EXPECT_TRUE(fs::exists("test_dir/subdir_copy/test.txt"));
}

TEST_F(FileManagerTest, MoveFile) {
    // Create a test file
    std::ofstream outfile("test_dir/test.txt");
    outfile << "test content" << std::endl;
    outfile.close();

    EXPECT_TRUE(fileManager_.MoveFile("test.txt", "test_move.txt"));
    EXPECT_FALSE(fs::exists("test_dir/test.txt"));
    EXPECT_TRUE(fs::exists("test_dir/test_move.txt"));
}

TEST_F(FileManagerTest, MoveDirectory) {
    // Create a test directory and file
    fs::create_directory("test_dir/subdir/");
    std::ofstream outfile("test_dir/subdir/test.txt");
    outfile << "test content" << std::endl;
    outfile.close();

    EXPECT_TRUE(fileManager_.MoveDirectory("subdir/", "subdir_move/"));
    EXPECT_FALSE(fs::exists("test_dir/subdir/"));
    EXPECT_TRUE(fs::exists("test_dir/subdir_move/test.txt"));
}

TEST_F(FileManagerTest, RemoveFile) {
    // Create a test file
    std::ofstream outfile("test_dir/test.txt");
    outfile << "test content" << std::endl;
    outfile.close();

    EXPECT_TRUE(fileManager_.RemoveFile("test.txt"));
    EXPECT_FALSE(fs::exists("test_dir/test.txt"));
}

TEST_F(FileManagerTest, RemoveDirectory) {
    // Create a test directory and file
    fs::create_directory("test_dir/subdir/");
    std::ofstream outfile("test_dir/subdir/test.txt");
    outfile << "test content" << std::endl;
    outfile.close();

    EXPECT_TRUE(fileManager_.RemoveDirectory("subdir/"));
    EXPECT_FALSE(fs::exists("test_dir/subdir/"));
}

TEST_F(FileManagerTest, GetFileList) {
    // Create test files
    std::ofstream outfile1("test_dir/test1.txt");
    outfile1 << "test content" << std::endl;
    outfile1.close();
    std::ofstream outfile2("test_dir/test2.txt");
    outfile2 << "test content" << std::endl;
    outfile2.close();

    std::vector<std::string> fileList = fileManager_.GetFileList();
    EXPECT_EQ(fileList.size(), 2);
    EXPECT_TRUE(std::find(fileList.begin(), fileList.end(), "test1.txt") != fileList.end());
    EXPECT_TRUE(std::find(fileList.begin(), fileList.end(), "test2.txt") != fileList.end());
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
