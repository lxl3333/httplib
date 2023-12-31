#ifndef REMOTEFILEMANAGER_H
#define REMOTEFILEMANAGER_H

#include <string>
#include <vector>
#include <jsoncpp/json/json.h>
#include "../include/httplib.h"

class RemoteFileManager
{
public:
    RemoteFileManager(std::shared_ptr<httplib::Client>& client);

    bool createRemoteFileOrDirectory(const std::string &filename,std::string token,bool isDirectory);
    bool renameRemoteFile(const std::string &filename, const std::string &newname,std::string token);
    bool moveRemoteFile(const std::string &filename, const std::string &newpath);
    bool copyRemoteFile(const std::string &filename, const std::string &targetpath);
    bool removeRemoteFile(const std::string &filename,std::string token);
    bool listRemoteFiles(const std::string &remotePath,std::string token,std::vector<std::pair<std::string, bool>> &files);
    bool checkFileExist(const std::string &filename,std::string token);
private:
    std::shared_ptr<httplib::Client> client_;
};

#endif // REMOTEFILEMANAGER_H
