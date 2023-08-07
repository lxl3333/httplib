#ifndef FTPSERVER_H
#define FTPSERVER_H

#include<src/base/Config.h>

class FtpServer{
public:
    FtpServer();
    void start();
    void run();
private:
    Config config_;
    
};






#endif
