#include "clientwindow.h"
#include "./ui_clientwindow.h"

#include <QPalette>

ClientWindow::ClientWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClientWindow)
{
    ui->setupUi(this);

    QPalette pal = this->palette(); // 设置背景图片
    pal.setBrush(QPalette::Window,QBrush(QPixmap(":/icon/bg.png")));
    setPalette(pal);
}

ClientWindow::~ClientWindow()
{
    delete ui;
}

