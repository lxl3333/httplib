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

    static Logger& getInstance(std::ostream& log_stream){
        static Logger instance(log_stream);
        return instance;
    }

    void Log(Logger::LogLevel level,std::string message);
    void Debug(std::string message);
    void Info(std::string message);
    void Warning(std::string message);
    void Error(std::string message);
private:
    explicit Logger(std::ostream& log_stream);
    ~Logger() =default;

    Logger(const Logger&)=delete;
    Logger& operator=(const Logger&) =delete;
    
    std::ostream& log_stream_;
};


#define LOG(level,message,log_stream) Logger::getInstance(log_stream).log(level,message)
#define LOG_Debug(message) Logger::getInstance(std::cout).Debug(message)
#define LOG_Info(message) Logger::getInstance(std::cout).Info(message)
#define LOG_Warning(message) Logger::getInstance(std::cout).Warning(message)
#define LOG_Error(message) Logger::getInstance(std::cout).Error(message)

#endif
