#include "FileManageRequest.h"

FileManageRequest::FileManageRequest(const std::string& rootPath) : fileManager_(rootPath) {}

bool FileManageRequest::handleCreateDirectory(const httplib::Request& req, httplib::Response& res) {
    std::string dirname = req.body;
    if (fileManager_.CreateDirectory(dirname)) {
        res.status = 200;
        res.set_content("Directory created successfully", "text/plain");
        return true;
    } else {
        res.status = 500;
        res.set_content("Failed to create directory", "text/plain");
        return false;
    }
    return false;
}

bool FileManageRequest::handleRenameFile(const httplib::Request& req, httplib::Response& res) {
    std::string filename = req.matches[1];
    std::string newname = req.body;
    if (fileManager_.MoveFile(filename, newname)) {
        res.status = 200;
        res.set_content("File or directory renamed successfully", "text/plain");
        return true;
    } else {
        res.status = 500;
        res.set_content("Failed to rename file or directory", "text/plain");
    }
    return false;
}

bool FileManageRequest::handleMoveFile(const httplib::Request& req, httplib::Response& res) {
    std::string filename = req.matches[1];
    std::string newpath = req.body;
    if (fileManager_.MoveFile(filename, newpath)) {
        res.status = 200;
        res.set_content("File or directory moved successfully", "text/plain");
        return true;
    } else {
        res.status = 500;
        res.set_content("Failed to move file or directory", "text/plain");
    }
    return false;
}

bool FileManageRequest::handleCopyFile(const httplib::Request& req, httplib::Response& res) {
    std::string filename = req.matches[1];
    std::string targetpath = req.body;
    if (fileManager_.CopyFile(filename, targetpath)) {
        res.status = 200;
        res.set_content("File or directory copied successfully", "text/plain");
        return true;
    } else {
        res.status = 500;
        res.set_content("Failed to copy file or directory", "text/plain");
    }
    return false;
}

bool FileManageRequest::handleRemoveFile(const httplib::Request& req, httplib::Response& res) {
    std::string filename = req.matches[1];
    std::string targetpath = req.body;
    if (fileManager_.RemoveFile(filename)) {
        res.status = 200;
        res.set_content("File or directory delete successfully", "text/plain");
        return true;
    } else {
        res.status = 500;
        res.set_content("Failed to delete file or directory", "text/plain");
    }
    return false;
}

// void FileManageRequest::handleSetPermissions(const httplib::Request& req, httplib::Response& res) {
//     std::string filename = req.matches[1];
//     std::string permissions = req.body;
//     if (fileManager_.SetPermissions(filename, permissions)) {
//         res.status = 200;
//         res.set_content("Permissions set successfully", "text/plain");
//     } else {
//         res.status = 500;
//         res.set_content("Failed to set permissions for file or directory", "text/plain");
//     }
// }
