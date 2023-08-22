#include "FileManageRequest.h"

#include <jsoncpp/json/json.h>

#include "../base/Logger.h"

FileManageRequest::FileManageRequest(const std::string &rootPath) : fileManager_(rootPath) {}

bool FileManageRequest::handleCreateDirectory(const httplib::Request &req, httplib::Response &res)
{
    std::string dirname = req.body;
    if (fileManager_.CreateDirectory(dirname))
    {
        res.status = 200;
        res.set_content("Directory created successfully", "text/plain");
        return true;
    }
    else
    {
        res.status = 500;
        res.set_content("Failed to create directory", "text/plain");
        return false;
    }
    return false;
}

bool FileManageRequest::handleRenameFile(const httplib::Request &req, httplib::Response &res)
{
    std::string filename = req.matches[1];
    std::string newname = req.body;
    if (fileManager_.MoveFile(filename, newname))
    {
        res.status = 200;
        res.set_content("File or directory renamed successfully", "text/plain");
        return true;
    }
    else
    {
        res.status = 500;
        res.set_content("Failed to rename file or directory", "text/plain");
    }
    return false;
}

bool FileManageRequest::handleMoveFile(const httplib::Request &req, httplib::Response &res)
{
    std::string filename = req.get_param_value("filename");
    std::string newpath = req.get_param_value("newpath");
    
    if (fileManager_.MoveFile(fileManager_.getRootPath()+filename, fileManager_.getRootPath()+newpath))
    {
        res.status = 200;
        res.set_content("File or directory moved successfully", "text/plain");
        return true;
    }
    else
    {
        res.status = 500;
        res.set_content("Failed to move file or directory", "text/plain");
        return false;
    }
}

bool FileManageRequest::handleCopyFile(const httplib::Request &req, httplib::Response &res)
{
    std::string filename = req.get_param_value("filename");
    std::string targetpath = req.get_param_value("targetpath");
    
    if (fileManager_.CopyFile(fileManager_.getRootPath()+filename,fileManager_.getRootPath()+targetpath))
    {
        res.status = 200;
        res.set_content("File or directory copied successfully", "text/plain");
        return true;
    }
    else
    {
        res.status = 500;
        res.set_content("Failed to copy file or directory", "text/plain");
        return false;
    }
}


bool FileManageRequest::handleRemoveFile(const httplib::Request &req, httplib::Response &res)
{
    Json::CharReaderBuilder reader;
    Json::Value root;

    // Parse JSON body from request
    std::string errors;
    std::istringstream jsonStream(req.body);
    bool parsingSuccessful = Json::parseFromStream(reader, jsonStream, &root, &errors);
    
    if (!parsingSuccessful) {
        res.status = 400; // Bad Request
        res.set_content("Invalid JSON format: " + errors, "text/plain");
        return false;
    }

    std::string remotePath = root["filename"].asString();
    LOG_Info("handleRemoveFile:" + remotePath);

    std::string fullPath = fileManager_.getRootPath() + remotePath;

    if (fileManager_.RemoveFile(fullPath))
    {
        res.status = 200;
        res.set_content("File or directory deleted successfully", "text/plain");
        return true;
    }
    else
    {
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

bool FileManageRequest::handleListFiles(const httplib::Request &req, httplib::Response &res)
{

    std::string remotePath = req.get_param_value("path"); // Assuming you pass the remote path as a query parameter

    LOG_Info("handleListFiles:"+remotePath);
    // Construct the full path using the rootPath and remotePath
    std::string fullPath = fileManager_.getRootPath() + remotePath;

    // Check if the path exists and is a directory
    if (!fileManager_.isDirectory(fullPath))
    {
        res.status = 400; // Bad Request
        res.set_content("Invalid directory path", "text/plain");
        return false;
    }

    // List files in the directory
    std::vector<std::pair<std::string, bool>> files;
    if (fileManager_.listFiles(fullPath, files))
    {
        Json::Value fileListJson(Json::arrayValue);
        for (const auto &item : files)
        {
            const std::string &fileName = item.first;
            bool isDirectory = item.second;

            Json::Value fileObject;
            fileObject["name"] = fileName;
            fileObject["type"] = isDirectory ? "folder" : "file";
            fileListJson.append(fileObject);
        }

        Json::StreamWriterBuilder writer;
        std::string jsonString = Json::writeString(writer, fileListJson);

        res.set_content(jsonString, "application/json");
        return true;
    }
    else
    {
        res.status = 500; // Internal Server Error
        res.set_content("Error listing files", "text/plain");
        return false;
    }
}


bool FileManageRequest::handleFileExists(const httplib::Request &req, httplib::Response &res) {
    std::string remotePath = req.get_param_value("path"); // Assuming you pass the remote path as a query parameter

    LOG_Info("handleFileExists:" + remotePath);
    // Construct the full path using the rootPath and remotePath
    std::string fullPath = fileManager_.getRootPath() + remotePath;

    // Check if the path exists and is a file
    if (fileManager_.FileExists(fullPath))
    {
        res.status = 200;
        res.set_content("File exists", "text/plain");
    }
    else
    {
        res.status = 404; // Not Found
        res.set_content("File not found", "text/plain");
    }
    
    return true;
}

