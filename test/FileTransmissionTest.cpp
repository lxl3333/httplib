#include <gtest/gtest.h>
#include "../src/Server/FileTransmission.h"
#include <fstream>

// Test fixture for FileTransmission tests
class FileTransmissionTest : public ::testing::Test {
protected:
  FileTransmission file_transmission;
};

TEST_F(FileTransmissionTest, SendFixedFile) {
  httplib::Request req;
  httplib::Response res;

  // Set file_path parameter
  req.params.emplace("file_path", "path/to/file.txt");

  // Call SendFixedFile
  bool result = file_transmission.SendFixedFile(req, res);

  // Check the result
  EXPECT_TRUE(result);
  EXPECT_EQ(res.status, 200);
  EXPECT_EQ(res.get_header_value("Content-Type"), "application/octet-stream");

  // Read the content of the response file
  std::ifstream file("path/to/file.txt", std::ios::binary);
  std::string expected_contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  file.close();

  EXPECT_EQ(res.body, expected_contents);
}

TEST_F(FileTransmissionTest, SendChunkedFile) {
  httplib::Request req;
  httplib::Response res;

  // Set file_path parameter
  req.params.emplace("file_path", "path/to/file.txt");

  // Call SendChunkedFile
  bool result = file_transmission.SendChunkedFile(req, res);

  // Check the result
  EXPECT_TRUE(result);
  EXPECT_EQ(res.status, 200);
  EXPECT_EQ(res.get_header_value("Content-Type"), "application/octet-stream");

  // Read the content of the response file
  std::ifstream file("path/to/file.txt", std::ios::binary);
  std::string expected_contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  file.close();

  EXPECT_EQ(res.body, expected_contents);
}

TEST_F(FileTransmissionTest, Sendc) {
  httplib::Request req;
  httplib::Response res;

  // Set file_path parameter
  req.params.emplace("file_path", "path/to/file.txt");

  // Call Sendc
  bool result = file_transmission.Sendc(req, res);

  // Check the result
  EXPECT_TRUE(result);
  EXPECT_EQ(res.status, 200);
  EXPECT_EQ(res.get_header_value("Content-Type"), "text/plain");

  // Read the content of the response file
  std::ifstream file("path/to/file.txt", std::ios::binary);
  std::string expected_contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  file.close();

  EXPECT_EQ(res.body, expected_contents);
}

TEST_F(FileTransmissionTest, ComputeHash) {
  std::string file_path = "path/to/file.txt";

  // Create a test file with known content
  std::ofstream file(file_path, std::ios::binary);
  file << "Test content";
  file.close();

  // Call ComputeHash
  std::string hash = file_transmission.ComputeHash(file_path);

  // Expected MD5 hash of "Test content"
  std::string expected_hash = "5a105e8b9d40e1329780d62ea2265d8a";

  // Check the computed hash
  EXPECT_EQ(hash, expected_hash);
}

TEST_F(FileTransmissionTest, VerifyHash) {
  std::string file_path = "path/to/file.txt";

  // Create a test file with known content
  std::ofstream file(file_path, std::ios::binary);
  file << "Test content";
  file.close();

  std::string expected_hash = "5a105e8b9d40e1329780d62ea2265d8a";

  // Call VerifyHash with correct expected hash
  bool result = file_transmission.VerifyHash(file_path, expected_hash);

  // Check the result
  EXPECT_TRUE(result);

  // Call VerifyHash with incorrect expected hash
  result = file_transmission.VerifyHash(file_path, "incorrect_hash");

  // Check the result
  EXPECT_FALSE(result);
}

// Add more tests as needed

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
