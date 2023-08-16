#include "clientwindow.h"
#include "./ui_clientwindow.h"

#include "../base/Singleton.h"
#include "../base/Logger.h"

#include <QPalette>
#include <QMessageBox>
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
    LOG_Debug("界面初始化");
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
        cloginmanager_->Logout();
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

