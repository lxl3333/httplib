#ifndef LOGGER_H
#define LOGGER_H

#include<iostream>
#include<string>

class Logger{
public:
    enum LogLevel
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
    };

    Logger(std::ostream& log_stream);
    void Log(int level,std::string message);
    void Debug(std::string message);
    void Info(std::string message);
    void Warning(std::string message);
    void Error(std::string message);
private:
    std::ostream& log_stream_;
};

#endif
