#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include "cloginmanager.h"
#include "../base/FileManager.h"
#include "../include/httplib.h"
#include <QWidget>
#include <QListWidgetItem>
QT_BEGIN_NAMESPACE
namespace Ui
{
    class ClientWindow;
}
QT_END_NAMESPACE

class ClientWindow : public QWidget
{
    Q_OBJECT

public:
    ClientWindow(QWidget *parent = nullptr);
    ~ClientWindow();
    void show_clientdir(const QString &path); // 显示客户端目录

private slots:
    void on_connect_clicked();

public slots:
    void onFolderItemClicked(QListWidgetItem *item);
    void goToParentDirectory();

private:
    Ui::ClientWindow *ui;
    std::shared_ptr<httplib::Client> client_;
    std::unique_ptr<CLoginManager> cloginmanager_;
    std::unique_ptr<FileManager> filemanager_;
    char clientdir[256];           // 客户端路径
    QString client_filename[1024]; // 客户端获取到的文件名
};
#endif // CLIENTWINDOW_H
