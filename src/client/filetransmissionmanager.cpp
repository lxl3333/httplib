#include "filetransmissionmanager.h"
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

FileTransmissionManager::FileTransmissionManager(std::shared_ptr<httplib::Client> client)
    :client_(client){}

bool FileTransmissionManager::UploadFixedFile(const std::string& filePath) {
    if (!fs::exists(filePath) || !fs::is_regular_file(filePath)) {
        // 文件不存在或不是普通文件
        return false;
    }

    size_t fileSize = fs::file_size(filePath);

    httplib::Request request;
    request.method = "POST";
    request.headers = {
        { "Content-Length", std::to_string(fileSize) },
        { "Content-Type", "application/octet-stream" }
    };
    request.body = fs::load_file(filePath);

    httplib::Response response;
    if (client_->send(request, response)) {
        if (response.status == 200) {
            // 上传成功
            return true;
        } else {
            // 上传失败
            return false;
        }
    } else {
        // 请求发送失败
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
