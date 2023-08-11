#ifndef FTPSERVER_H
#define FTPSERVER_H

#include<string>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "../base/Config.h"
#include "HandlerFactory.h"
#include "../include/httplib.h"
#include "../base/Logger.h"
#include "../base/Singleton.h"
class FtpServer {
public:
    FtpServer(std::string path);

    void start();

private:
    Config config_;
    httplib::Server server_;
    void configureServer(httplib::Server& server);
};

#endif


