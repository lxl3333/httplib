#include "clientwindow.h"
#include "./ui_clientwindow.h"
#include "../base/Logger.h"
#include "../base/Singleton.h"

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
    Singleton<Logger>::getInstance().Debug("界面初始化");
}

ClientWindow::~ClientWindow()
{
    delete ui;
}


void ClientWindow::on_connect_clicked()
{
    QString _connect = ui->connect->text();
    if(_connect != "连接")
    {
        client_->stop();
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

    const char* ip = _ip.toStdString().c_str();
    short port = _port.toShort();

    client_=std::make_shared<httplib::Client>(ip,port);  // 连接
    cloginmanager_ =std::make_unique<CLoginManager>(client_);
    if(client_->Get("/")->status!=200)
    {
        qDebug("client connect failed!");
        return;
    }

    if(cloginmanager_->Login(_username.toStdString(),_password.toStdString())){

    }else{
        QMessageBox::critical(nullptr,"登陆失败","用户名或秘密错误，请重试！");
    }
    ui->connect->setText("断开");
}

