#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class FileManager {
public:
    FileManager(const std::string& path);

    bool FileExists(const std::string& filename) const;
    bool CreateDirectory(const std::string& dirname) const;
    bool CreateFile(const std::string& filename) const;
    bool CopyFile(const std::string& srcFilename, const std::string& destFilename) const;
    bool CopyDirectory(const std::string& srcDirname, const std::string& destDirname) const;
    bool MoveFile(const std::string& srcFilename, const std::string& destFilename) const;
    bool MoveDirectory(const std::string& srcDirname, const std::string& destDirname) const;
    bool RemoveFile(const std::string& filename) const;
    bool RemoveDirectory(const std::string& dirname) const;
    std::vector<std::string> GetFileList() const;

private:
    std::string path_;
};

#endif // FILE_MANAGER_H