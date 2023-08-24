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
  bool ReceiveChunkedFile(httplib::Request &req,httplib::Response &res);
  bool ReceiveDirectory(httplib::Request &req,httplib::Response &res);

//private:
  std::string ComputeHash(const std::string& path);
  bool VerifyHash(const std::string& path, const std::string& expected_hash);
};

#endif
