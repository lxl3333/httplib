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
    std::string requestPath = "/files/move"; // Assuming the server API endpoint is "/move_file"

    httplib::Params params;
    params.emplace("filename", filename);
    params.emplace("newpath", newpath);
    
    httplib::Headers headers = {
        {"Content-Type", "application/x-www-form-urlencoded"}
    };

    // Make a POST request to the server
    auto response = client_->Post(requestPath.c_str(), headers, params);

    if (response && response->status == 200)
    {
        // Check the response content for success or failure
        if (response->body == "File or directory moved successfully")
        {
            return true;
        }
        else
        {
            // Handle the failure case
            return false;
        }
    }
    
    return false; // Return false if the request failed or the response was not as expected
}

bool RemoteFileManager::copyRemoteFile(const std::string &filename, const std::string &targetpath)
{
    std::string requestPath = "/files/copy"; // Assuming the server API endpoint is "/copy_file"

    httplib::Params params;
    params.emplace("filename", filename);
    params.emplace("targetpath", targetpath);
    
    httplib::Headers headers = {
        {"Content-Type", "application/x-www-form-urlencoded"}
    };

    // Make a POST request to the server
    auto response = client_->Post(requestPath.c_str(), headers, params);

    if (response && response->status == 200)
    {
        // Check the response content for success or failure
        if (response->body == "File or directory copied successfully")
        {
            return true;
        }
        else
        {
            // Handle the failure case
            return false;
        }
    }
    
    return false; // Return false if the request failed or the response was not as expected
}


bool RemoteFileManager::removeRemoteFile(const std::string& filename) {
    std::string url = "/files/delete"; // Replace with the actual API endpoint

    httplib::Headers headers = {
        {"Content-Type", "application/json"}
    };

    Json::Value requestData;
    requestData["filename"] = filename;

    Json::StreamWriterBuilder writer;
    std::string jsonRequestBody = Json::writeString(writer, requestData);

    httplib::Result result = client_->Delete(url.c_str(), headers, jsonRequestBody, "application/json");

    if (result && result->status == 200) {
        // Check the response from the server if the file was successfully removed
        // Update the return value accordingly
        return true;
    } else {
        // Handle error and return false
        LOG_Info("Handle error and return false");
        return false;
    }
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

bool RemoteFileManager::checkFileExist(const std::string &filename, std::string token)
{
    std::string requestPath = "/files/CheckFileExist"; // Assuming the server API endpoint is "/check_file_exist"

    httplib::Params params;
    params.emplace("token", token);
    params.emplace("filename", filename);
    
    httplib::Headers headers = {
        {"Content-Type", "application/x-www-form-urlencoded"}
    };

    // Make a POST request to the server
    auto response = client_->Post(requestPath, headers, params);

    if (response && response->status == 200)
    {
        // Check the response content to determine if the file exists
        if (response->body == "File exists")
        {
            return true;
        }
        else if (response->body == "File not found")
        {
            return false;
        }
        // Handle other response content as needed
    }
    
    return false; // Return false if the request failed or the response was not as expected
}
