#ifndef FILEMANAGERESTQUEST_H
#define FILEMANAGERESTQUEST_H

#include <string>
#include "../include/httplib.h"
#include "../base/FileManager.h"

class FileManageRequest
{
public:
    FileManageRequest(const std::string &rootPath);

    bool handleCreateFileOrDirectory(const httplib::Request &req, httplib::Response &res);
    bool handleRenameFile(const httplib::Request &req, httplib::Response &res);
    bool handleMoveFile(const httplib::Request &req, httplib::Response &res);
    bool handleCopyFile(const httplib::Request &req, httplib::Response &res);
    bool handleRemoveFile(const httplib::Request &req, httplib::Response &res);
    //    void handleSetPermissions(const httplib::Request& req, httplib::Response& res);
    bool handleListFiles(const httplib::Request &req, httplib::Response &res);
    bool handleFileExists(const httplib::Request &req, httplib::Response &res);
private:
    FileManager fileManager_;
};

#endif // FILEMANAGERESTQUEST_H
