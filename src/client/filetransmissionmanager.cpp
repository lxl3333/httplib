#include "filetransmissionmanager.h"
#include <fstream>
#include "../base/Logger.h"

namespace fs = std::filesystem;

FileTransmissionManager::FileTransmissionManager(std::shared_ptr<httplib::Client> client)
    :client_(client){}

bool FileTransmissionManager::UploadFixedFile(const std::string& remotePath, const std::string& localPath,const std::string fileName, std::string token) {
    LOG_Info("UploadFixedFile");
    std::string url = "/files/upload/FixedFile";

    httplib::Headers headers = httplib::Headers{
        {"token", token},
        {"remotePath", remotePath},
        {"fileName",fileName},
        {"Content-Type", "application/octet-stream"}  // 设置请求头
    };

    std::ifstream file(localPath, std::ios::binary);
    if (!file.is_open()) {
        // Handle error: unable to open the local file
        return false;
    }

    // Calculate file size
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Initialize buffer
    constexpr size_t BUFFER_SIZE = 4096;
    std::vector<char> buffer(BUFFER_SIZE);
    LOG_Info("ready to post");
    if (auto result = client_->Post(url, headers,fileSize,
        [&](size_t offset,size_t length, httplib::DataSink& sink) {
            size_t bytesToRead = std::min(BUFFER_SIZE, fileSize);
            if (bytesToRead > 0) {
                file.read(buffer.data(), bytesToRead);
                sink.write(buffer.data(), bytesToRead);
                fileSize -= bytesToRead;
            } else {
                sink.done();
            }
            return true;
        },
            "application/octet-stream")) {
        // Handle success
        return true;
    } else {
        // Handle error
        return false;
    }
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
