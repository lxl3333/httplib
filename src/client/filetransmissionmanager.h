#ifndef FILETRANSMISSIONMANAGER_H
#define FILETRANSMISSIONMANAGER_H

#include <string>
#include "../include/httplib.h"

class FileTransmissionManager {
public:
    FileTransmissionManager(std::shared_ptr<httplib::Client> client);

    bool UploadFixedFile(const std::string& remotePath,const std::string& localPath,const std::string fileName,std::string token);
    bool UploadChunkedFile(const std::string& remotePath, const std::string& localPath, const std::string& fileName, std::string token) ;
    bool DownloadFixedFile(const std::string& remotePath,
                           const std::string& localPath);
    bool DownloadChunkedFile(const std::string& remotePath,
                             const std::string& localPath);

private:
    std::shared_ptr<httplib::Client> client_;
};

#endif // FILETRANSMISSIONMANAGER_H
