#include "clientwindow.h"
#include "./ui_clientwindow.h"

#include "../base/Singleton.h"
#include "../base/Logger.h"

#include <QPalette>
#include <QMessageBox>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <iostream>

ClientWindow::ClientWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClientWindow)
    ,client_(nullptr)
    ,cloginmanager_(nullptr)

{
    ui->setupUi(this);
    QPalette pal = this->palette(); // 设置背景图片
    pal.setBrush(QPalette::Window,QBrush(QPixmap(":/icon/bg.png")));
    setPalette(pal);
    //Singleton<Logger>::getInstance(std::cout).Debug("界面初始化");
    strcpy(clientdir,".");
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
    LOG_Debug("开始"+_connect.toStdString());
    if(_connect != "连接")
    {
        LOG_Debug("ClognManager::logout");
        cloginmanager_->Logout();
        LOG_Debug("ClognManager::logout");
        ui->connect->setText("连接");
        ui->serverdir->clear();
        ui->listWidget_s->clear();
        return;
    }

    QString _ip = ui->ip->text();   // 获取控件的text
    QString _port = ui->port->text();
    QString _username = ui->username->text();   // 获取控件的text
    QString _password = ui->password->text();

    std::string ip = _ip.toStdString();
    int port = _port.toInt();

    LOG_Debug(ip+" "+_port.toStdString());
    client_=std::make_shared<httplib::Client>(ip,port);  // 连接
    cloginmanager_ =std::make_unique<CLoginManager>(client_);
    LOG_Debug("httplib::client创建客户端");
    if (!client_) {
        qDebug("client is null!");
        return;
    }

    auto response = client_->Get("/");
    if(!response){
        LOG_Debug("client connect failed!");
        return ;
    }
    if (response->status != 200) {
        LOG_Debug("server didn't esit!");
        return;
    }
    LOG_Debug("服务器存在");
    if(cloginmanager_->Login(_username.toStdString(),_password.toStdString())){
        LOG_Debug("创建登陆成功");
    }else{
        QMessageBox::critical(nullptr,"登陆失败","用户名或秘密错误，请重试！");
        return ;
    }
    ui->connect->setText("断开");
}

// 显示客户端的所有文件
void ClientWindow::show_clientdir()
{
    ui->clientdir->clear(); // 清空clientdir
    char cur_dir[1024];
    char* temp = getcwd(cur_dir, sizeof(cur_dir)-1); // 获取当前路径
    if(temp == NULL)
    {
        qDebug("获取当前路径失败");
    }

    qDebug("%s",cur_dir);
    ui->clientdir->setText(cur_dir);    // 设置clientdir的text


    int i = 0;
    DIR* dp = opendir(clientdir);
    for(struct dirent* file = readdir(dp); file!=NULL; file = readdir(dp))
    {
        client_filename[i++] = file->d_name;

        char img[256] = {};

        strcpy(img,":/icon/");
        if(file->d_type == DT_DIR)  // 判断是否是目录文件
        {
            strcat(img,"dir.png");
            //qDebug("dir");
        }
        else
        {
            strcat(img,"file.png");
        }
        QIcon icon(img);
        QListWidgetItem* item = new QListWidgetItem(icon,file->d_name);
        ui->listWidget_c->addItem(item);    // 添加字段
    }
}
