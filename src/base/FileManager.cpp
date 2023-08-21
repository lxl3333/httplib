#include "FileManager.h"

FileManager::FileManager(const std::string &path) : path_(path) {}

bool FileManager::FileExists(const std::string &filename) const
{
    return fs::exists(filename);
}

bool FileManager::CreateDirectory(const std::string &dirname) const
{
    return fs::create_directory(path_ + dirname);
}

bool FileManager::CreateFile(const std::string &filename) const
{
    std::ofstream outfile(path_ + filename);
    return outfile.good();
}

bool FileManager::CopyFile(const std::string &srcFilename, const std::string &destFilename) const
{
    try
    {
        fs::copy(path_ + srcFilename, path_ + destFilename);
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error copying file: " << e.what() << std::endl;
        return false;
    }
}

bool FileManager::CopyDirectory(const std::string &srcDirname, const std::string &destDirname) const
{
    try
    {
        fs::copy(path_ + srcDirname, path_ + destDirname, fs::copy_options::recursive);
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error copying directory: " << e.what() << std::endl;
        return false;
    }
}

bool FileManager::MoveFile(const std::string &srcFilename, const std::string &destFilename) const
{
    try
    {
        fs::rename(path_ + srcFilename, path_ + destFilename);
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error moving file: " << e.what() << std::endl;
        return false;
    }
}

bool FileManager::MoveDirectory(const std::string &srcDirname, const std::string &destDirname) const
{
    try
    {
        fs::rename(path_ + srcDirname, path_ + destDirname);
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error moving directory: " << e.what() << std::endl;
        return false;
    }
}

bool FileManager::RemoveFile(const std::string &filename) const
{
    try
    {
        fs::remove(filename);
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error removing file: " << e.what() << std::endl;
        return false;
    }
}

bool FileManager::RemoveDirectory(const std::string &dirname) const
{
    try
    {
        fs::remove_all(path_ + dirname);
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error removing directory: " << e.what() << std::endl;
        return false;
    }
}

std::vector<std::string> FileManager::GetFileList() const
{
    std::vector<std::string> fileList;
    for (const auto &entry : fs::directory_iterator(path_))
    {
        fileList.push_back(entry.path().filename().string());
    }
    return fileList;
}

std::string FileManager::getRootPath() const
{
    return path_;
}

bool FileManager::isDirectory(const std::string &path) const
{
    return fs::is_directory(path);
}

bool FileManager::listFiles(const std::string &path, std::vector<std::pair<std::string, bool>> &files) const
{
    try
    {
        for (const auto &entry : fs::directory_iterator(path))
        {
            if (entry.is_regular_file() || entry.is_directory())
            {
                bool isDirectory = entry.is_directory();
                files.push_back(std::make_pair(entry.path().filename().string(), isDirectory));
            }
        }
        return true;
    }
    catch (const fs::filesystem_error &e)
    {
        std::cerr << "Error listing files: " << e.what() << std::endl;
        return false;
    }
}

std::string FileManager::getParentDirectory(const std::string &path) const
{
    fs::path fsPath(path);
    fs::path parentPath = fsPath.parent_path();

    if (parentPath != fs::path()) // Check if it's not the root directory
    {
        return parentPath.string();
    }

    return ""; // Return empty string if it's the root directory
}
