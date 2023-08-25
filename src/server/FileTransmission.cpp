#include "FileTransmission.h"
#include <openssl/md5.h>
#include <fstream>
#include <iostream>

#include "../base/Logger.h"

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

bool FileTransmission::UploadFixedFile(const httplib::Request& req, httplib::Response& res,std::string rootPath) {
    std::string file_path = req.get_header_value("remotePath");
    std::string file_name = req.get_header_value("fileName");
    LOG_Info("UploadFixedFile:"+rootPath+file_path+"/"+file_name);
    std::ofstream file(rootPath+file_path+"/"+file_name, std::ios::binary);

    if (!file) {
        res.status = 500;
        res.set_content("Failed to create file", "text/plain");
        return false;
    }

    // Get the content length from the request headers
    size_t content_length = std::stoul(req.get_header_value("Content-Length"));

    // Read the content of the specified length from the request body and write to the file
    std::string content = req.body.substr(0, content_length);
    file.write(content.c_str(), content_length);

    res.status = 200; 
    res.set_content("File received", "text/plain");
    return true;
}


bool FileTransmission::UploadChunkedFile(const httplib::Request& req, httplib::Response& res,std::string rootPath) {
    std::string remotePath = req.get_header_value("remotePath");
    std::string fileName = req.get_header_value("fileName");
    LOG_Info("UploadChunkedFile"+rootPath+remotePath + "/" + fileName);
    std::ofstream file(rootPath+remotePath + "/" + fileName, std::ios::binary);
    if (!file) {
        res.status = 500;
        res.set_content("Failed to create file", "text/plain");
        return false;
    }
    LOG_Info(req.body);
    std::istringstream body_stream(req.body);
    std::string line;
    size_t chunk_size = 0;

    // 逐行读取块的大小
    while (std::getline(body_stream, line)) {
        // 将十六进制的块大小转换为整数
        chunk_size = std::stoul(line, nullptr, 16);

        if (chunk_size == 0) {
            break; // 数据块传输结束
        }

        char buffer[4096];
        while (chunk_size > 0) {
            // 计算要读取的大小
            size_t read_size = std::min(chunk_size, sizeof(buffer));
            
            // 从流中读取数据块
            body_stream.read(buffer, read_size);
            
            // 将读取的数据块写入文件
            file.write(buffer, read_size);
            
            // 更新剩余块大小
            chunk_size -= read_size;
        }

        // 读取并丢弃块末尾的 CRLF（回车换行）
        body_stream.ignore(2);
    }


    res.status=200;
    res.set_content("File received", "text/plain");
    return true;
}

bool FileTransmission::Upload(const httplib::Request& req, httplib::Response& res, std::string rootPath) {
    if (req.has_header("Content-Length")) {
        // Content-Length header is set, indicating a fixed size file upload
        return UploadFixedFile(req, res, rootPath);
    } else {
        // Content-Length header is not set, indicating a chunked file upload
        return UploadChunkedFile(req, res, rootPath);
    }
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
