#include "FileManager.h"

FileManager::FileManager(const std::string& path) : path_(path) {}

bool FileManager::FileExists(const std::string& filename) const {
    return fs::exists(path_ + filename);
}

bool FileManager::CreateDirectory(const std::string& dirname) const {
    return fs::create_directory(path_ + dirname);
}

bool FileManager::CreateFile(const std::string& filename) const {
    std::ofstream outfile(path_ + filename);
    return outfile.good();
}

bool FileManager::CopyFile(const std::string& srcFilename, const std::string& destFilename) const {
    try {
        fs::copy(path_ + srcFilename, path_ + destFilename);
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error copying file: " << e.what() << std::endl;
        return false;
    }
}

bool FileManager::CopyDirectory(const std::string& srcDirname, const std::string& destDirname) const {
    try {
        fs::copy(path_ + srcDirname, path_ + destDirname, fs::copy_options::recursive);
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error copying directory: " << e.what() << std::endl;
        return false;
    }
}

bool FileManager::MoveFile(const std::string& srcFilename, const std::string& destFilename) const {
    try {
        fs::rename(path_ + srcFilename, path_ + destFilename);
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error moving file: " << e.what() << std::endl;
        return false;
    }
}

bool FileManager::MoveDirectory(const std::string& srcDirname, const std::string& destDirname) const {
    try {
        fs::rename(path_ + srcDirname, path_ + destDirname);
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error moving directory: " << e.what() << std::endl;
        return false;
    }
}

bool FileManager::RemoveFile(const std::string& filename) const {
    try {
        fs::remove(path_ + filename);
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error removing file: " << e.what() << std::endl;
        return false;
    }
}

bool FileManager::RemoveDirectory(const std::string& dirname) const {
    try {
        fs::remove_all(path_ + dirname);
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error removing directory: " << e.what() << std::endl;
        return false;
    }
}

std::vector<std::string> FileManager::GetFileList() const {
    std::vector<std::string> fileList;
    for (const auto& entry : fs::directory_iterator(path_)) {
        fileList.push_back(entry.path().filename().string());
    }
    return fileList;
}
