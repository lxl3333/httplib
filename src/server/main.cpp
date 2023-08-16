#include"FtpServer.h"
int main()
{
    FtpServer ftpserver("config/config.json");
    ftpserver.start();
}
