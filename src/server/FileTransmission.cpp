#include "FileTransmission.h"
#include <openssl/md5.h>
#include <fstream>
#include <iostream>

FileTransmission::FileTransmission() {}

bool FileTransmission::SendFixedFile(httplib::Request& req, httplib::Response& res) {
  std::string file_path = req.get_param_value("file_path");
  std::ifstream file(file_path, std::ios::binary);

  if (!file) {
    res.status = 404;
    res.set_content("File not found", "text/plain");
    return false;
  }

  std::string file_contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  res.set_content(file_contents, "application/octet-stream");
  return true;
}

bool FileTransmission::SendChunkedFile(httplib::Request& req, httplib::Response& res) {
  std::string file_path = req.get_param_value("file_path");
  std::ifstream file(file_path, std::ios::binary);

  if (!file) {
    res.status = 404;
    res.set_content("File not found", "text/plain");
    return false;
  }

  auto content_provider = [file_path](size_t offset, httplib::DataSink& sink) -> bool {
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
      return false;
    }
    file.seekg(offset, std::ios::beg);
    char buffer[4096];
    if (file.eof()) {
      return false;
    }
    file.read(buffer, sizeof(buffer));
    auto bytesRead = file.gcount();

    sink.write(buffer, bytesRead);
    return true;
  };

  res.set_chunked_content_provider(
    /* content_type */ "application/octet-stream",
    content_provider);
  return true;
}

bool FileTransmission::Sendc(httplib::Request& req, httplib::Response& res) {
  std::string file_path = req.get_param_value("file_path");
  std::ifstream file(file_path, std::ios::binary);

  if (!file) {
    res.status = 404;
    res.set_content("File not found", "text/plain");
    return false;
  }

  std::string file_contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  res.set_content(file_contents, "text/plain");
  return true;
}

bool FileTransmission::ReceiveFixedFile(httplib::Request& req, httplib::Response& res) {
  std::string file_path = req.get_param_value("file_path");
  std::ofstream file(file_path, std::ios::binary);

  if (!file) {
    res.status = 500;
    res.set_content("Failed to create file", "text/plain");
    return false;
  }

  file << req.body;
  res.set_content("File received", "text/plain");
  return true;
}

bool FileTransmission::ReceiveChunkedFile(httplib::Request& req, httplib::Response& res) {
  std::string file_path = req.get_param_value("file_path");
  std::ofstream file(file_path, std::ios::binary);

  if (!file) {
    res.status = 500;
    res.set_content("Failed to create file", "text/plain");
    return false;
  }

  file << req.body;
  res.set_content("File received", "text/plain");
  return true;
}

bool FileTransmission::ReceiveDirectory(httplib::Request& req, httplib::Response& res) {
  std::string dir_path = req.get_param_value("dir_path");

  if (dir_path.empty()) {
    res.status = 400;
    res.set_content("Directory path not provided", "text/plain");
    return false;
  }

  // Implementation to receive and save directory contents
  // ...

  res.set_content("Directory received", "text/plain");
  return true;
}

std::string FileTransmission::ComputeHash(const std::string& path) {
  std::ifstream file(path, std::ios::binary);
  if (!file) {
    return "";
  }

  MD5_CTX md5Context;
  MD5_Init(&md5Context);

  char buffer[4096];
  while (file.read(buffer, sizeof(buffer))) {
    MD5_Update(&md5Context, buffer, sizeof(buffer));
  }

  MD5_Update(&md5Context, buffer, file.gcount());

  unsigned char hash[MD5_DIGEST_LENGTH];
  MD5_Final(hash, &md5Context);

  char hash_str[33];
  for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
    sprintf(hash_str + (i * 2), "%02x", hash[i]);
  }

  return std::string(hash_str);
}

bool FileTransmission::VerifyHash(const std::string&path, const std::string& expected_hash) {
  std::string computed_hash = ComputeHash(path);
  return computed_hash == expected_hash;
}
