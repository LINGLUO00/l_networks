#include<iostream>
#include<fstream>
#include<cstdlib>
#include<ctime>
#include<cstring>
#include<cstdarg>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>

enum
{
    Debug=0,
    Info,
    Warn,
    Error,
    Fatal,
};

enum
{
    Screen=0,
    OneFile,
    ClassFile,
};

std::string LevelToString(int level)
{
    switch(level)
    {
        case Debug:
            return "Debug";
        case Info:
            return "Info";
        case Warn:
            return "Warn";
        case Error:
            return "Error";
        case Fatal:
            return "Fatal";
        default:
            return "Unkown";
    }
}

const int defaultstyle=Screen;
const std::string default_filename="log";
const std::string default_dir="log";

class Log
{
public:
    Log():_style(defaultstyle),_filename(default_filename)
    {
        mkdir(default_dir.c_str(),0775);
    }
    ~Log(){}

    void SetStyle(int style)
    {
        _style=style;
    }
    std::string GetTime()
    {
        time_t currstamp=time(nullptr);
        struct tm*curr=localtime(&currstamp);
        char time_buffer[128];
        snprintf(time_buffer,sizeof(time_buffer),"%d-%d-%d:%d:%d:%d",curr->tm_year+1900,curr->tm_mon+1,curr->tm_mday,curr->tm_hour,curr->tm_min,curr->tm_sec);
        return time_buffer;

    }
    void WriteLogToOneFile(const std::string& logname,const std::string& message)
    {
        umask(0);
        int fd=open(logname.c_str(),O_CREAT | O_WRONLY | O_APPEND,0666);
        if(fd<0) std::cerr<<"create log file error"<<std::endl;
        write(fd,message.c_str(),message.size());
        close(fd);
    }
    void WriteLogToClassFile(const std::string& levelstr,const std::string& message)
    {
        std::string logname=default_dir+"/"+default_filename+levelstr+".log";
        WriteLogToOneFile(logname,message);

    }
    void Printlog(const std::string&levelstr,const std::string& message)
    {
        switch(_style)
        {
            case Screen:
                std::cout<<message<<std::endl;
                break;
            case OneFile:
                WriteLogToClassFile("all",message);
                break;
            case ClassFile:
                WriteLogToClassFile(levelstr,message);
                break;
            default:
                break;
        }
    }
    void LogMessage(int level,const char*format,...)
    {
        char sysmsg_buffer[1024];
        std::string currtime=GetTime();
        std::string levelstr=LevelToString(level);
        std::string idstr=std::to_string(getpid());
        snprintf(sysmsg_buffer,sizeof(sysmsg_buffer),"[%s][%s][%s]",levelstr.c_str(),currtime.c_str(),idstr.c_str());

        char logmsg_buffer[1024];
        va_list args;
        va_start(args,format);
        vsnprintf(logmsg_buffer,sizeof(logmsg_buffer),format,args);
        va_end(args);

        std::string loginfo=sysmsg_buffer;
        loginfo+=": ";
        loginfo+=logmsg_buffer;
        Printlog(levelstr,loginfo);


    }

private:
    int _style;
    std::string _filename;
};

Log log;
