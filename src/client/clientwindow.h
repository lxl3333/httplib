#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include "cloginmanager.h"
#include "../base/FileManager.h"
#include "../include/httplib.h"
#include "remotefilemanager.h"
#include "clipboardmanager.h"

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
    void show_clientdir(const QString path = ""); // 显示客户端目录
    void show_serverdir(const QString path);

private slots:
    void on_connect_clicked();

public slots:
    void onFolderItemDoubleClicked(QListWidgetItem *item);
    void onFolderItemLeftDoubleClicked(QListWidgetItem *item);
    void onsFolderItemDoubleClicked(QListWidgetItem *item);
    void onsFolderItemLeftDoubleClicked(QListWidgetItem *item);

    void showContextMenu(const QPoint &pos);
    void showsContextMenu(const QPoint &pos);

    void goToParentDirectory();

private:
    Ui::ClientWindow *ui;
    std::shared_ptr<httplib::Client> client_;
    std::unique_ptr<CLoginManager> cloginmanager_;
    std::unique_ptr<RemoteFileManager> remotefilemanager_;
    std::unique_ptr<FileManager> filemanager_;
    std::vector<std::pair<std::string, bool>> remotefiles;
    std::unique_ptr<ClipboardManager> clipboardmanager_;
    std::unique_ptr<ClipboardManager> serverclipboardmanager_;
};
#endif // CLIENTWINDOW_H
