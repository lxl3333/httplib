#ifndef CLIPBOARDMANAGER_H
#define CLIPBOARDMANAGER_H

#include <string>

class ClipboardManager {
public:
    ClipboardManager();

    void copyFile(const std::string &filePath);
    void cutFile(const std::string &filePath);
    bool isFileCopied() const;
    bool isFileCut() const;
    std::string getCopyFilePath() const;
    std::string getCutFilePath() const;
    void clearCopyFile();
    void clearCutFile();

private:
    bool copied_;
    bool cut_;
    std::string copiedFilePath_;
    std::string cutFilePath_;
};

#endif // CLIPBOARDMANAGER_H
