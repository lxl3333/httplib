#ifndef REMOTEFILEMANAGER_H
#define REMOTEFILEMANAGER_H

#include <string>
#include <vector>
#include <jsoncpp/json/json.h>
#include <httplib.h>

class RemoteFileManager
{
public:
    RemoteFileManager(const std::string &serverBaseUrl);

    bool createRemoteDirectory(const std::string &dirname);
    bool renameRemoteFile(const std::string &filename, const std::string &newname);
    bool moveRemoteFile(const std::string &filename, const std::string &newpath);
    bool copyRemoteFile(const std::string &filename, const std::string &targetpath);
    bool removeRemoteFile(const std::string &filename);
    bool listRemoteFiles(const std::string &remotePath, std::vector<std::pair<std::string, bool>> &files);

private:
    std::string serverBaseUrl_;
    httplib::Client client_;
};

#endif // REMOTEFILEMANAGER_H
