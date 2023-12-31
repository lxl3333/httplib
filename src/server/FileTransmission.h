#ifndef FILETRANSMISSION_H
#define FILETRANSMISSION_H

#include <string>
#include "../include/httplib.h"


class FileTransmission {
public:
  FileTransmission();

  bool SendFixedFile(httplib::Request &req,httplib::Response &res);
  bool SendChunkedFile(httplib::Request &req,httplib::Response &res);
  bool Sendc(httplib::Request &req,httplib::Response &res);
  bool UploadFixedFile(const httplib::Request &req,httplib::Response &res,std::string rootPath);
  bool UploadChunkedFile(const httplib::Request &req,httplib::Response &res,std::string rootPath);

  bool Upload(const httplib::Request &req,httplib::Response &res,std::string rootPath);

//private:
  std::string ComputeHash(const std::string& path);
  bool VerifyHash(const std::string& path, const std::string& expected_hash);
};

#endif
