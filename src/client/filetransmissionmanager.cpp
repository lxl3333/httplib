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


bool FileTransmissionManager::UploadChunkedFile(const std::string& remotePath, const std::string& localPath, const std::string& fileName, std::string token) {
    LOG_Info("UploadChunkedFile");
    std::string url = "/files/upload/ChunkedFile";

    httplib::Headers headers = httplib::Headers{
        {"token", token},
        {"remotePath", remotePath},
        {"fileName", fileName},
        {"Content-Type", "application/octet-stream"}  // 设置请求头
    };

    std::ifstream file(localPath, std::ios::binary);
    if (!file.is_open()) {
        // Handle error: unable to open the local file
        return false;
    }

    // Initialize buffer
    constexpr size_t BUFFER_SIZE = 4096;
    std::vector<char> buffer(BUFFER_SIZE);

    LOG_Info("ready to post");
    if (auto result = client_->Post(url, headers,
        [&](size_t offset, httplib::DataSink& sink) {
            // Seek to the specified offset in the file
            //file.seekg(offset);

            // Read data into the buffer
            std::vector<char> buffer(BUFFER_SIZE);
            file.read(buffer.data(), BUFFER_SIZE);
            std::streamsize bytesRead = file.gcount();

            if (bytesRead > 0) {
                // Convert chunk size to hex and write to sink
                // std::string chunkSizeStr = std::to_string(bytesRead);
                // std::string hexChunkSize = std::to_string(chunkSizeStr.size()) + "\r\n";
                // sink.write(hexChunkSize.c_str(), hexChunkSize.size());
                std::ostringstream hexStream;
                hexStream << std::hex << bytesRead;
                std::string hexChunkSize = hexStream.str() + "\r\n";
                sink.write(hexChunkSize.c_str(), hexChunkSize.size());
                // Write data and chunk delimiter to sink
                sink.write(buffer.data(), bytesRead);
                sink.write("\r\n", 2);

                return true;  // Continue reading and sending more chunks
            } else {
                // Write final chunk of size 0 to signal end
                sink.write("0\r\n\r\n", 5);
                sink.done();
                return false; // Stop reading and sending more chunks
            }
        },
        "application/octet-stream")) {
        // Handle success
        return true;
    } else {
        // Handle error
        return false;
    }


}


bool FileTransmissionManager::DownloadFixedFile(const std::string& remotePath,
                                                const std::string& localPath) {
    // ... 实现下载定长文件的逻辑 ...
}

bool FileTransmissionManager::DownloadChunkedFile(const std::string& remotePath,
                                                  const std::string& localPath) {
    // ... 实现下载变长文件的逻辑 ...
}
