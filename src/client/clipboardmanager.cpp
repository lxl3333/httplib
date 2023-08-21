#include "clipboardmanager.h"

ClipboardManager::ClipboardManager() : copied_(false), cut_(false) {}

void ClipboardManager::copyFile(const std::string &filePath) {
    copied_ = true;
    cut_ = false;
    copiedFilePath_ = filePath;
    cutFilePath_ = "";
}

void ClipboardManager::cutFile(const std::string &filePath) {
    copied_ = false;
    cut_ = true;
    copiedFilePath_ = "";
    cutFilePath_ = filePath;
}

bool ClipboardManager::isFileCopied() const {
    return copied_;
}

bool ClipboardManager::isFileCut() const {
    return cut_;
}

std::string ClipboardManager::getCopyFilePath() const {
    return copiedFilePath_;
}

std::string ClipboardManager::getCutFilePath() const {
    return cutFilePath_;
}

void ClipboardManager::clearCopyFile() {
    copied_ = false;
    copiedFilePath_ = "";
}

void ClipboardManager::clearCutFile() {
    cut_ = false;
    cutFilePath_ = "";
}


// 添加一个方法来判断是否有文件被剪切
bool ClipboardManager::hasCutFile() const
{
    return cut_;
}

// 添加一个方法来判断是否有文件被复制
bool ClipboardManager::hasCopiedFile() const
{
    return copied_;
}
