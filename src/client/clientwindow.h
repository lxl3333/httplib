#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include "cloginmanager.h"
#include "../include/httplib.h"
#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class ClientWindow; }
QT_END_NAMESPACE

class ClientWindow : public QWidget
{
    Q_OBJECT

public:
    ClientWindow(QWidget *parent = nullptr);
    ~ClientWindow();

private slots:
    void on_connect_clicked();

private:
    Ui::ClientWindow *ui;
    std::shared_ptr<httplib::Client> client_;
    std::unique_ptr<CLoginManager> cloginmanager_;
};
#endif // CLIENTWINDOW_H
