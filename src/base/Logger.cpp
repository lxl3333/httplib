#include"Logger.h"

Logger::Logger(std::ostream& log_stream)
    :log_stream_(log_stream)
{

}

void Logger::Log(Logger::LogLevel level,std::string message){
    std::string level_string;
    switch(level){
        case LogLevel::DEBUG:
            level_string="DEBUG";
            break;
        case LogLevel::INFO:
            level_string="INFO";
            break;
        case LogLevel::WARNING:
            level_string="WARNING";
            break;
        case LogLevel::ERROR:
            level_string="ERROR";
            break;
    }
    log_stream_<<"["<<level_string<<"]"<<message<<std::endl;
}
void Logger::Debug(std::string message){
    Log(LogLevel::DEBUG,message);
}
void Logger::Info(std::string message){
    Log(LogLevel::INFO,message);
}
void Logger::Warning(std::string message){
    Log(LogLevel::WARNING,message);
}
void Logger::Error(std::string message){
    Log(LogLevel::ERROR,message);
}
