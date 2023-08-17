#include "clientwindow.h"
#include "./ui_clientwindow.h"

#include "../base/Singleton.h"
#include "../base/Logger.h"

#include <QPalette>
#include <QMessageBox>
#include <QDir>

ClientWindow::ClientWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::ClientWindow), client_(nullptr), cloginmanager_(nullptr), filemanager_(std::make_unique<FileManager>("/home/scutech"))

{
    ui->setupUi(this);
    QPalette pal = this->palette(); // 设置背景图片
    pal.setBrush(QPalette::Window, QBrush(QPixmap(":/icon/bg.png")));
    setPalette(pal);
    // Singleton<Logger>::getInstance(std::cout).Debug("界面初始化");
    connect(ui->listWidget_c, &QListWidget::itemDoubleClicked, this, &ClientWindow::onFolderItemClicked);
    //connect(ui->goToParentButton, &QPushButton::clicked, this, &ClientWindow::goToParentDirectory);
    strcpy(clientdir, ".");
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
    }
    else
    {
        QMessageBox::critical(nullptr, "登陆失败", "用户名或秘密错误，请重试！");
        return;
    }
    ui->connect->setText("断开");
}

void ClientWindow::show_clientdir(const QString path )
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

void ClientWindow::onFolderItemClicked(QListWidgetItem *item)
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
    QString parentPath =QString::fromStdString(filemanager_->getParentDirectory(currentPath.toStdString()));

    if (!parentPath.isEmpty())
    {
        show_clientdir(parentPath); // 使用上一级目录路径来更新文件列表和当前目录显示
    }
    else
    {
        qDebug("已经在根目录，无法返回上一层");
    }
}


void ClientWindow::show_serverdir(const QString path,std::vector<std::pair<std::string, bool>> files)
{
    ui->serverdir->clear(); // 清空clientdir

    // 将当前路径设置为 clientdir 的文本
    ui->serverdir->setText(path);

    ui->listWidget_s->clear(); // 清空listWidget_c

    // 添加上一级目录的项目
    QListWidgetItem *parentItem = new QListWidgetItem("..");
    QIcon parentIcon(":/icon/parent.png");
    parentItem->setIcon(parentIcon);
    ui->listWidget_s->addItem(parentItem);

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
