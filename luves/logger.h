//
//  log.hpp
//  Luves
//
//  Created by leviathan on 16/6/17.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>


namespace luves
{
    extern std::string strlevel[];
    
    //日志等级
    enum loglevel
    {
        LDEBUG, //指明细致的事件信息，对调试应用最有用。
        LERROR, //指明错误事件，但应用可能还能继续运行。
        LFATAL, //指明非常严重的错误事件，可能会导致应用终止执行。
        LINFO,  //指明描述信息，从粗粒度上描述了应用运行过程。
        LTRACE, //比DEBUG级别的粒度更细。
        LWARN,  //指明潜在的有害状况。

    };
    
    
#define printlog(level,...) \
    do { \
        if (level<=Logger::Instance().GetLevel()) { \
            Logger::Instance().PrintLog(level, __FILE__, __LINE__, __func__, __VA_ARGS__); \
        } \
    } while(0)
    
    
#define TRACE_LOG(...) printlog(LTRACE, __VA_ARGS__)
#define DEBUG_LOG(...) pringlog(LDEBUG, __VA_ARGS__)
#define INFO_LOG(...) printlog(LINFO, __VA_ARGS__)
#define WARN_LOG(...) printlog(LWARN, __VA_ARGS__)
#define ERROR_LOG(...) printlog(LERROR, __VA_ARGS__)
#define FATAL_LOG(...) printlog(LFATAL, __VA_ARGS__)
#define FATALIF_LOG(b, ...) do { if((b)) { printlog(LFATAL, __VA_ARGS__); } } while (0)
#define CHECK_LOG(b, ...) do { if((b)) { printlog(LFATAL, __VA_ARGS__); } } while (0)
#define EXITIF_LOG(b, ...) do { if ((b)) { printlog(LERROR, __VA_ARGS__); _exit(1); }} while(0)
    
#define SETLOGLEVEL(l) Log::Instance().SetLevel(l)
#define SETLOGFILE(n) Log::Instance().SetFileName(n)
    
    //日志模式,分为终端打印与文件输出两种,默认终端打印
    enum logmode
    {
        TERMIANAL,
        FILE
    };
    
    
    //
    //日志模块,单例模式
    //
    class Logger
    {
    public:
        static Logger & Instance()
        {
            static Logger log;
            return log;
        }
        void SetLogMode(logmode mode){mode_=mode;};
        void PrintLog(int level,const char * file,int line,const char * func ,const char * parm...);
        void SetLevel(const std::string & level);
        void SetLevel(loglevel level){level_=level;}
        
        loglevel & GetLevel(){return level_;}
        void SetFileName(const std::string & filename);
    private:
        Logger();
        ~Logger();
        Logger & operator=(Logger const &);   // assign op. hidden
        Logger(Logger const &);               // copy ctor hidden
        std::string filename_;
        loglevel level_;                //log level
        int fd_;                        //file descriptor
        logmode mode_;
        
    };
}

#endif /* log_h */
