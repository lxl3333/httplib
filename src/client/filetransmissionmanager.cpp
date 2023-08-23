#include "filetransmissionmanager.h"
#include <fstream>

FileTransmissionManager::FileTransmissionManager(std::shared_ptr<httplib::Client> client)
    :client_(client){}

bool FileTransmissionManager::UploadFixedFile(const std::string& filePath) {;
    // ... 实现上传定长文件的逻辑 ...
}

bool FileTransmissionManager::UploadChunkedFile(const std::string& filePath) {
    // ... 实现上传变长文件的逻辑 ...
}

bool FileTransmissionManager::DownloadFixedFile(const std::string& remotePath,
                                                const std::string& localPath) {
    // ... 实现下载定长文件的逻辑 ...
}

bool FileTransmissionManager::DownloadChunkedFile(const std::string& remotePath,
                                                  const std::string& localPath) {
    // ... 实现下载变长文件的逻辑 ...
}
