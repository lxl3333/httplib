#include "clientwindow.h"
#include "./ui_clientwindow.h"

#include "../base/Singleton.h"
#include "../base/Logger.h"

#include <QPalette>
#include <QMessageBox>
#include <QDir>
#include <QMouseEvent>
#include <QMenu>



ClientWindow::ClientWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::ClientWindow), client_(nullptr), cloginmanager_(nullptr), filemanager_(std::make_unique<FileManager>("/home/scutech")), remotefilemanager_(nullptr)
    ,clipboardmanager_(std::make_unique<ClipboardManager>())

{
    ui->setupUi(this);
    QPalette pal = this->palette(); // 设置背景图片
    pal.setBrush(QPalette::Window, QBrush(QPixmap(":/icon/bg.png")));
    setPalette(pal);
    // Singleton<Logger>::getInstance(std::cout).Debug("界面初始化");
    // connect(ui->listWidget_c, &QListWidget::itemDoubleClicked, this, &ClientWindow::onFolderItemDoubleClicked);
    connect(ui->listWidget_s, &QListWidget::itemDoubleClicked, this, &ClientWindow::onsFolderItemDoubleClicked);
    connect(ui->listWidget_c, &QListWidget::itemDoubleClicked, this, &ClientWindow::onFolderItemDoubleClicked);

    ui->listWidget_c->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget_c, &QListWidget::customContextMenuRequested, this, &ClientWindow::showContextMenu);

    ui->listWidget_s->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget_s, &QListWidget::customContextMenuRequested, this, &ClientWindow::showsContextMenu);
    // connect(ui->goToParentButton, &QPushButton::clicked, this, &ClientWindow::goToParentDirectory);
    LOG_Debug("界面初始化");
    show_clientdir();
}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::on_connect_clicked()
{
    QString _connect = ui->connect->text();
    LOG_Debug("开始" + _connect.toStdString());
    if (_connect != "连接")
    {
        LOG_Debug("ClognManager::logout");
        cloginmanager_->Logout();
        LOG_Debug("ClognManager::logout");
        ui->connect->setText("连接");
        ui->serverdir->clear();
        ui->listWidget_s->clear();
        return;
    }

    QString _ip = ui->ip->text(); // 获取控件的text
    QString _port = ui->port->text();
    QString _username = ui->username->text(); // 获取控件的text
    QString _password = ui->password->text();

    std::string ip = _ip.toStdString();
    int port = _port.toInt();

    LOG_Debug(ip + " " + _port.toStdString());
    client_ = std::make_shared<httplib::Client>(ip, port); // 连接
    cloginmanager_ = std::make_unique<CLoginManager>(client_);
    remotefilemanager_ = std::make_unique<RemoteFileManager>(client_);
    LOG_Debug("httplib::client创建客户端");
    if (!client_)
    {
        qDebug("client is null!");
        return;
    }

    auto response = client_->Get("/");
    if (!response)
    {
        LOG_Debug("client connect failed!");
        return;
    }
    if (response->status != 200)
    {
        LOG_Debug("server didn't esit!");
        return;
    }
    LOG_Debug("服务器存在");
    if (cloginmanager_->Login(_username.toStdString(), _password.toStdString()))
    {
        LOG_Debug("创建登陆成功");
        QString path("/");
        show_serverdir(path);
    }
    else
    {
        QMessageBox::critical(nullptr, "登陆失败", "用户名或秘密错误，请重试！");
        return;
    }
    ui->connect->setText("断开");
}

void ClientWindow::show_clientdir(const QString path)
{
    ui->clientdir->clear(); // 清空clientdir

    QString currentPathQString;

    if (path.isEmpty())
    {
        // 使用程序执行路径作为默认路径
        fs::path currentPath = fs::current_path();
        std::string currentPathStr = currentPath.string();

        currentPathQString = QString::fromStdString(currentPathStr);
    }
    else
    {
        currentPathQString = path;
    }

    // 将当前路径设置为 clientdir 的文本
    ui->clientdir->setText(currentPathQString);

    std::vector<std::pair<std::string, bool>> files;
    if (filemanager_->listFiles(currentPathQString.toStdString(), files))
    {
        ui->listWidget_c->clear(); // 清空listWidget_c

        // 添加上一级目录的项目
        QListWidgetItem *parentItem = new QListWidgetItem("..");
        QIcon parentIcon(":/icon/parent.png");
        parentItem->setIcon(parentIcon);
        ui->listWidget_c->addItem(parentItem);

        for (const auto &item : files)
        {
            const std::string &fileName = item.first;
            bool isDirectory = item.second;

            QString itemText = QString::fromStdString(fileName);
            std::string img = ":/icon/";

            if (isDirectory)
            {
                img += "dir.png";
            }
            else
            {
                img += "file.png";
            }

            QIcon icon(QString::fromStdString(img));
            QListWidgetItem *listItem = new QListWidgetItem(icon, itemText);
            ui->listWidget_c->addItem(listItem); // 添加字段
        }
    }
    else
    {
        qDebug("获取文件列表失败");
    }
}

void ClientWindow::onFolderItemLeftDoubleClicked(QListWidgetItem *item)
{
    QString clickedItemText = item->text();

    // Check if the clicked item is a folder
    if (!clickedItemText.isEmpty())
    {
        QString folderName = clickedItemText;
        QString currentPath = ui->clientdir->text();

        if (folderName == "..") // Handle going to parent directory
        {
            LOG_Info("go to parentDirectory");
            QString parentPath = QString::fromStdString(filemanager_->getParentDirectory(currentPath.toStdString()));
            if (!parentPath.isEmpty())
            {
                ui->clientdir->setText(parentPath);
                show_clientdir(parentPath);
            }
            else
            {
                LOG_Debug("无法获取上一级目录");
            }
            return;
        }

        // Construct the full path of the clicked item
        QString clickedPath = currentPath + QDir::separator() + folderName;

        if (filemanager_->isDirectory(clickedPath.toStdString())) // Check if it's a directory
        {
            LOG_Info("go to childrenDirectory");
            show_clientdir(clickedPath);
        }
    }
}

void ClientWindow::goToParentDirectory()
{
    QString currentPath = ui->clientdir->text();
    QString parentPath = QString::fromStdString(filemanager_->getParentDirectory(currentPath.toStdString()));

    if (!parentPath.isEmpty())
    {
        show_clientdir(parentPath); // 使用上一级目录路径来更新文件列表和当前目录显示
    }
    else
    {
        qDebug("已经在根目录，无法返回上一层");
    }
}

void ClientWindow::show_serverdir(const QString path)
{
    ui->serverdir->clear(); // 清空serverdir
    LOG_Info("清空serverdir");
    // 将当前路径设置为 clientdir 的文本
    ui->serverdir->setText(path);

    ui->listWidget_s->clear(); // 清空listWidget_s

    LOG_Info("清空listWidget_s");
    // 添加上一级目录的项目
    QListWidgetItem *parentItem = new QListWidgetItem("..");
    QIcon parentIcon(":/icon/parent.png");
    parentItem->setIcon(parentIcon);
    ui->listWidget_s->addItem(parentItem);
    LOG_Info("添加上一级目录的项目");
    std::vector<std::pair<std::string, bool>> files;

    remotefilemanager_->listRemoteFiles(path.toStdString(), cloginmanager_->GetToken(), files);

    remotefiles = files;

    for (const auto &item : files)
    {
        const std::string &fileName = item.first;
        bool isDirectory = item.second;

        QString itemText = QString::fromStdString(fileName);
        std::string img = ":/icon/";

        if (isDirectory)
        {
            img += "dir.png";
        }
        else
        {
            img += "file.png";
        }

        QIcon icon(QString::fromStdString(img));
        QListWidgetItem *listItem = new QListWidgetItem(icon, itemText);
        ui->listWidget_s->addItem(listItem); // 添加字段
    }
}

void ClientWindow::onsFolderItemLeftDoubleClicked(QListWidgetItem *item)
{
    QString clickedItemText = item->text();

    // Check if the clicked item is a folder
    if (!clickedItemText.isEmpty())
    {
        QString folderName = clickedItemText;
        QString currentPath = ui->serverdir->text();

        if (folderName == "..") // Handle going to parent directory
        {
            LOG_Info("go to parentDirectory");
            auto findparentdir = [](const std::string &remotePath) -> std::string
            {
                std::string parentPath = remotePath;
                size_t lastSlashPos = parentPath.find_last_of('/');
                if (lastSlashPos != std::string::npos)
                {
                    parentPath = parentPath.substr(0, lastSlashPos);
                }
                return parentPath;
            };
            QString parentPath = QString::fromStdString(findparentdir(currentPath.toStdString()));
            if (!parentPath.isEmpty())
            {
                ui->serverdir->setText(parentPath);
                show_serverdir(parentPath);
            }
            else
            {
                LOG_Debug("无法获取上一级目录");
            }
            return;
        }

        // Construct the full path of the clicked item
        QString clickedPath = currentPath + QDir::separator() + folderName;

        for (auto &item : remotefiles)
        {
            const std::string &fileName = item.first;
            bool isDirectory = item.second;
            if (fileName == folderName.toStdString() && isDirectory)
            {
                show_serverdir(clickedPath);
                break;
            }
        }
    }
}

void ClientWindow::onFolderItemDoubleClicked(QListWidgetItem *item)
{
    Qt::MouseButtons mouseButton = QApplication::mouseButtons();

    if (mouseButton == Qt::LeftButton)
    {
        onFolderItemLeftDoubleClicked(item);
    }
    // 其他鼠标双击逻辑
    // 在这里处理其他鼠标按钮的双击事件，例如右键双击等
    // 调用原来的itemDoubleClicked信号处理逻辑
    // else Q_EMIT ui->listWidget_c->itemDoubleClicked(item);
}

void ClientWindow::onsFolderItemDoubleClicked(QListWidgetItem *item)
{
    Qt::MouseButtons mouseButton = QApplication::mouseButtons();

    if (mouseButton == Qt::LeftButton)
    {
        onsFolderItemLeftDoubleClicked(item);
    }
    // 其他鼠标双击逻辑
    // 在这里处理其他鼠标按钮的双击事件，例如右键双击等
    // 调用原来的itemDoubleClicked信号处理逻辑
    // else Q_EMIT ui->listWidget_c->itemDoubleClicked(item);
}





void ClientWindow::showContextMenu(const QPoint &pos)
{
    QListWidgetItem *item = ui->listWidget_c->itemAt(pos);
    if (item)
    {
        QMenu contextMenu(this);
        QAction *actionOpen = contextMenu.addAction("Open");
        QAction *actionCut = contextMenu.addAction("Cut");
        QAction *actionCopy = contextMenu.addAction("Copy");
        QAction *actionPaste = contextMenu.addAction("Paste");
        QAction *actionDelete = contextMenu.addAction("Delete");

        // 在菜单中添加分隔线
        contextMenu.addSeparator();

        // 添加其他自定义功能
        QAction *actionCustom = contextMenu.addAction("Custom Action");

        //调节显示粘贴键
        if (clipboardmanager_->hasCutFile() || clipboardmanager_->hasCopiedFile())
        {
            actionPaste->setEnabled(true); // 可以粘贴
        }
        else
        {
            actionPaste->setEnabled(false); // 不可粘贴（灰色）
            QFont font = actionPaste->font();
            font.setItalic(true); // 将字体设置为斜体
            actionPaste->setFont(font);
        }

        QPalette pal = contextMenu.palette();
        pal.setColor(QPalette::Disabled, QPalette::Text, Qt::gray);
        contextMenu.setPalette(pal);
        
        // 显示上下文菜单并获取所选操作
        QAction *selectedAction = contextMenu.exec(QCursor::pos());



        QString clickedItemText = item->text();

        QString folderName = clickedItemText;
        QString currentPath = ui->clientdir->text();

        if(folderName=="..") return;

        QString clickedPath = currentPath + QDir::separator() + folderName;

        // Check if the clicked item is a folder
        if (selectedAction == actionOpen)
        {
            if (filemanager_->FileExists(clickedPath.toStdString()))
            {
                // 使用系统默认的文本编辑器打开文件
                std::string command = "xdg-open " + clickedPath.toStdString(); // 对于 Linux/Unix 系统
                int result = system(command.c_str());
                if (result == 0)
                {
                    std::cout << "File opened: " << clickedPath.toStdString() << std::endl;
                }
                else
                {
                    std::cout << "Failed to open file: " << clickedPath.toStdString() << std::endl;
                }
            }
            else
            {
                std::cout << "File not found: " << clickedPath.toStdString() << std::endl;
            }
        }
        // else if (selectedAction == actionCut)
        // {
        //     std::string filename; // 你需要指定要剪切的文件名
        //     if (fileManager.FileExists(filename))
        //     {
        //         // 执行剪切文件的逻辑
        //         // 假设你有一个剪贴板对象 clipboard
        //         clipboard.cutFile(filename);
        //     }
        //     else
        //     {
        //         std::cout << "File not found: " << filename << std::endl;
        //     }
        // }
        // else if (selectedAction == actionCopy)
        // {
        //     std::string filename; // 你需要指定要复制的文件名
        //     if (fileManager.FileExists(filename))
        //     {
        //         // 执行复制文件的逻辑
        //         // 假设你有一个剪贴板对象 clipboard
        //         clipboard.copyFile(filename);
        //     }
        //     else
        //     {
        //         std::cout << "File not found: " << filename << std::endl;
        //     }
        // }
        // else if (selectedAction == actionPaste)
        // {
        //     // 执行粘贴文件的逻辑
        //     // 假设你有一个剪贴板对象 clipboard
        //     if (clipboard.isFileCut())
        //     {
        //         std::string sourceFile = clipboard.getCutFilePath();
        //         std::string destinationDirectory; // 你需要指定粘贴目标目录
        //         if (fileManager.isDirectory(destinationDirectory))
        //         {
        //             std::string destinationPath = destinationDirectory + "/" + fs::path(sourceFile).filename().string();
        //             if (fileManager.MoveFile(sourceFile, destinationPath))
        //             {
        //                 clipboard.clearCutFile();
        //             }
        //             else
        //             {
        //                 std::cout << "Failed to paste cut file." << std::endl;
        //             }
        //         }
        //         else
        //         {
        //             std::cout << "Invalid destination directory." << std::endl;
        //         }
        //     }
        //     else if (clipboard.isFileCopied())
        //     {
        //         std::string sourceFile = clipboard.getCopyFilePath();
        //         std::string destinationDirectory; // 你需要指定粘贴目标目录
        //         if (fileManager.isDirectory(destinationDirectory))
        //         {
        //             std::string destinationPath = destinationDirectory + "/" + fs::path(sourceFile).filename().string();
        //             if (fileManager.CopyFile(sourceFile, destinationPath))
        //             {
        //                 clipboard.clearCopyFile();
        //             }
        //             else
        //             {
        //                 std::cout << "Failed to paste copied file." << std::endl;
        //             }
        //         }
        //         else
        //         {
        //             std::cout << "Invalid destination directory." << std::endl;
        //         }
        //     }
        //     else
        //     {
        //         std::cout << "No file to paste in clipboard." << std::endl;
        //     }
        // }
        else if (selectedAction == actionDelete)
        {
            if (filemanager_->FileExists(clickedPath.toStdString()))
            {
                // 执行删除文件的逻辑
                if (filemanager_->RemoveFile(clickedPath.toStdString()))
                {
                    std::cout << "File deleted: " << clickedPath.toStdString() << std::endl;
                    show_clientdir(currentPath);
                }
                else
                {
                    std::cout << "Failed to delete file: " << clickedPath.toStdString() << std::endl;
                }
            }
            else
            {
                std::cout << "File not found: " << clickedPath.toStdString() << std::endl;
            }
        }
        else if (selectedAction == actionCustom)
        {
            // 处理自定义操作的逻辑
            // 例如：根据用户需求执行相应的自定义操作
        }

    }
}

void ClientWindow::showsContextMenu(const QPoint &pos)
{
    QListWidgetItem *item = ui->listWidget_s->itemAt(pos);
    if (item)
    {
        QMenu contextMenu(this);
        QAction *actionOpen = contextMenu.addAction("Open");
        QAction *actionCut = contextMenu.addAction("Cut");
        QAction *actionCopy = contextMenu.addAction("Copy");
        QAction *actionPaste = contextMenu.addAction("Paste");
        QAction *actionDelete = contextMenu.addAction("Delete");

        // 在菜单中添加分隔线
        contextMenu.addSeparator();

        // 添加其他自定义功能
        QAction *actionCustom = contextMenu.addAction("Custom Action");

        // 显示上下文菜单并获取所选操作
        QAction *selectedAction = contextMenu.exec(QCursor::pos());

        QString clickedItemText = item->text();

        QString folderName = clickedItemText;
        QString currentPath = ui->serverdir->text();

        if(folderName=="..") return;

        QString clickedPath = currentPath + QDir::separator() + folderName;

        // Check if the clicked item is a folder
        if (selectedAction == actionOpen)
        {
            if (filemanager_->FileExists(clickedPath.toStdString()))
            {
                // 使用系统默认的文本编辑器打开文件
                std::string command = "xdg-open " + clickedPath.toStdString(); // 对于 Linux/Unix 系统
                int result = system(command.c_str());
                if (result == 0)
                {
                    std::cout << "File opened: " << clickedPath.toStdString() << std::endl;
                }
                else
                {
                    std::cout << "Failed to open file: " << clickedPath.toStdString() << std::endl;
                }
            }
            else
            {
                std::cout << "File not found: " << clickedPath.toStdString() << std::endl;
            }
        }
        // else if (selectedAction == actionCut)
        // {
        //     std::string filename; // 你需要指定要剪切的文件名
        //     if (fileManager.FileExists(filename))
        //     {
        //         // 执行剪切文件的逻辑
        //         // 假设你有一个剪贴板对象 clipboard
        //         clipboard.cutFile(filename);
        //     }
        //     else
        //     {
        //         std::cout << "File not found: " << filename << std::endl;
        //     }
        // }
        // else if (selectedAction == actionCopy)
        // {
        //     std::string filename; // 你需要指定要复制的文件名
        //     if (fileManager.FileExists(filename))
        //     {
        //         // 执行复制文件的逻辑
        //         // 假设你有一个剪贴板对象 clipboard
        //         clipboard.copyFile(filename);
        //     }
        //     else
        //     {
        //         std::cout << "File not found: " << filename << std::endl;
        //     }
        // }
        // else if (selectedAction == actionPaste)
        // {
        //     // 执行粘贴文件的逻辑
        //     // 假设你有一个剪贴板对象 clipboard
        //     if (clipboard.isFileCut())
        //     {
        //         std::string sourceFile = clipboard.getCutFilePath();
        //         std::string destinationDirectory; // 你需要指定粘贴目标目录
        //         if (fileManager.isDirectory(destinationDirectory))
        //         {
        //             std::string destinationPath = destinationDirectory + "/" + fs::path(sourceFile).filename().string();
        //             if (fileManager.MoveFile(sourceFile, destinationPath))
        //             {
        //                 clipboard.clearCutFile();
        //             }
        //             else
        //             {
        //                 std::cout << "Failed to paste cut file." << std::endl;
        //             }
        //         }
        //         else
        //         {
        //             std::cout << "Invalid destination directory." << std::endl;
        //         }
        //     }
        //     else if (clipboard.isFileCopied())
        //     {
        //         std::string sourceFile = clipboard.getCopyFilePath();
        //         std::string destinationDirectory; // 你需要指定粘贴目标目录
        //         if (fileManager.isDirectory(destinationDirectory))
        //         {
        //             std::string destinationPath = destinationDirectory + "/" + fs::path(sourceFile).filename().string();
        //             if (fileManager.CopyFile(sourceFile, destinationPath))
        //             {
        //                 clipboard.clearCopyFile();
        //             }
        //             else
        //             {
        //                 std::cout << "Failed to paste copied file." << std::endl;
        //             }
        //         }
        //         else
        //         {
        //             std::cout << "Invalid destination directory." << std::endl;
        //         }
        //     }
        //     else
        //     {
        //         std::cout << "No file to paste in clipboard." << std::endl;
        //     }
        // }
        else if (selectedAction == actionDelete)
        {
            // if (filemanager_->FileExists(clickedPath.toStdString()))
            // {
                // 执行删除文件的逻辑
                if (remotefilemanager_->removeRemoteFile(clickedPath.toStdString()))
                {
                    LOG_Info( "File deleted: " + clickedPath.toStdString());
                    show_serverdir(currentPath);
                }
                else
                {
                    LOG_Info( "Failed to delete file: " + clickedPath.toStdString());
                }
            // }
            // else
            // {
            //     std::cout << "File not found: " << clickedPath.toStdString() << std::endl;
            // }
        }
        else if (selectedAction == actionCustom)
        {
            // 处理自定义操作的逻辑
            // 例如：根据用户需求执行相应的自定义操作
        }
    }
}


