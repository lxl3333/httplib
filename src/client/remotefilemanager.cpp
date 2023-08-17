#include "remotefilemanager.h"

#include "../base/Logger.h"

RemoteFileManager::RemoteFileManager(std::shared_ptr<httplib::Client>& client)
    : client_(client) {}

bool RemoteFileManager::createRemoteDirectory(const std::string &dirname)
{
    // Implementation for creating directory on server
}

bool RemoteFileManager::renameRemoteFile(const std::string &filename, const std::string &newname)
{
    // Implementation for renaming file on server
}

bool RemoteFileManager::moveRemoteFile(const std::string &filename, const std::string &newpath)
{
    // Implementation for moving file on server
}

bool RemoteFileManager::copyRemoteFile(const std::string &filename, const std::string &targetpath)
{
    // Implementation for copying file on server
}

bool RemoteFileManager::removeRemoteFile(const std::string &filename)
{
    // Implementation for removing file on server
}

// bool RemoteFileManager::listRemoteFiles(const std::string &remotePath, std::vector<std::pair<std::string, bool>> &files)
// {
//     // Implementation for listing files on server
// }

bool RemoteFileManager::listRemoteFiles(const std::string &remotePath ,std::string token, std::vector<std::pair<std::string, bool>> &files)
{
    std::string requestPath = "/files/ListFiles"; // Assuming the server API endpoint is "/list_files"

    httplib::Params params;
    params.emplace("token", token);
    params.emplace("path", remotePath);
    // Make a Post request to the server
    auto response = client_->Post(requestPath,params);

    if (response && response->status == 200)
    {
        // Parse the response body as JSON
        Json::Value root;
        Json::CharReaderBuilder reader;
        std::string errors;
        std::istringstream responseBody(response->body);
        if (!Json::parseFromStream(reader, responseBody, &root, &errors))
        {
            LOG_Info("Error parsing JSON response");
            return false;
        }

        // Extract file information from the JSON response
        LOG_Info("Extract file information from the JSON response");
        if (root.isArray())
        {
            for (const auto &file : root)
            {
                std::string fileName = file["name"].asString();
                bool isDirectory = (file["type"].asString() == "folder");
                files.push_back(std::make_pair(fileName, isDirectory));
            }
            return true;
        }
        else
        {
            // Invalid JSON response format
            LOG_Info("Invalid JSON response format");
            return false;
        }
    }
    else
    {
        // Error making the request or receiving the response
        return false;
    }

}
