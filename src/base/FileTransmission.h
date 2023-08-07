#ifndef FILETRANSMISSION_H
#define FILETRANSMISSION_H

#include <string>
#include "../include/httplib.h"


class FileTransmission {
public:
  FileTransmission();

  bool SendFixedFile(httplib::Request &req,httplib::Response &res);
  bool SendChunkedFile(const std::string& path, const std::string& dest_ip, int dest_port);
  bool SendDirectory(const std::string& path, const std::string& dest_ip, int dest_port);
  bool ReceiveFixedFile(const std::string& path, const std::string& src_ip, int src_port);
  bool ReceiveChunkedDirectory(const std::string& path, const std::string& src_ip, int src_port);

private:
  std::string ComputeHash(const std::string& path);
  bool VerifyHash(const std::string& path, const std::string& expected_hash);
};

#endif
